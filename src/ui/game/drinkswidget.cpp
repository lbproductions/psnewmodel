#include "drinkswidget.h"
#include "ui_drinkswidget.h"

#include "data/player.h"
#include "data/drink.h"
#include "model/drinkslistmodel.h"
#include "model/playerslistmodel.h"
#include "misc/tools.h"
#include "data/game.h"
#include "data/livedrink.h"
#include "data/round.h"

#include <QPersistence.h>

#include <QKeyEvent>
#include <QStandardItemModel>

/*********************************************************************
 * DrinksSortFilterModel
 */
class DrinksSortFilterModel : public QpSortFilterProxyObjectModel<Drink>
{
public:
    explicit DrinksSortFilterModel(QpObjectListModel<Drink> *sourceModel, QObject *parent = 0);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
    bool lessThan(QSharedPointer<Drink> left, QSharedPointer<Drink> right) const Q_DECL_OVERRIDE;
};

DrinksSortFilterModel::DrinksSortFilterModel(QpObjectListModel<Drink> *sourceModel, QObject *parent) :
    QpSortFilterProxyObjectModel<Drink>(sourceModel, parent)
{
}

bool DrinksSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool DrinksSortFilterModel::lessThan(QSharedPointer<Drink> left, QSharedPointer<Drink> right) const
{
    int countLeft = left->count();
    int countRight = right->count();

    return countLeft > countRight;
}

/*********************************************************************
 * PlayersSortFilterCheckableModel
 */
class DrinkPlayersModel : public QpSortFilterProxyObjectModel<Player>
{
public:
    explicit DrinkPlayersModel(QpObjectListModel<Player> *sourceModel, QObject *parent = 0);

    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    void clearCheckStates();

    QSharedPointer<Drink> drink() const;
    void setDrink(const QSharedPointer<Drink> &drink);

    QList<QSharedPointer<Player> > checkedPlayers();

protected:
    bool lessThan(QSharedPointer<Player> left, QSharedPointer<Player> right) const Q_DECL_OVERRIDE;

    QHash<QSharedPointer<Player>, Qt::CheckState> m_checkStates;
    QSharedPointer<Drink> m_drink;
};

DrinkPlayersModel::DrinkPlayersModel(QpObjectListModel<Player> *sourceModel, QObject *parent) :
    QpSortFilterProxyObjectModel<Player>(sourceModel, parent)
{
}

bool DrinkPlayersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole) {
        m_checkStates.insert(objectByIndex(index), static_cast<Qt::CheckState>(value.value<int>()));
        return true;
    }

    return QpSortFilterProxyObjectModel<Player>::setData(index, value, role);
}

QVariant DrinkPlayersModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::CheckStateRole) {
        return m_checkStates.value(objectByIndex(index), Qt::Unchecked);
    }

    return QpSortFilterProxyObjectModel<Player>::data(index, role);
}

Qt::ItemFlags DrinkPlayersModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QpSortFilterProxyObjectModel<Player>::flags(index);
    f |= Qt::ItemIsUserCheckable;
    return f;
}

void DrinkPlayersModel::clearCheckStates()
{
    beginResetModel();
    m_checkStates.clear();
    endResetModel();
}

bool DrinkPlayersModel::lessThan(QSharedPointer<Player> left, QSharedPointer<Player> right) const
{
    return QpSortFilterProxyObjectModel<Player>::lessThan(left, right);
}

QSharedPointer<Drink> DrinkPlayersModel::drink() const
{
    return m_drink;
}

void DrinkPlayersModel::setDrink(const QSharedPointer<Drink> &drink)
{
    beginResetModel();
    m_drink = drink;
    endResetModel();
}

QList<QSharedPointer<Player> > DrinkPlayersModel::checkedPlayers()
{
    QList<QSharedPointer<Player> > result;
    foreach(QSharedPointer<Player> player, sourceModel()->objects()) {
        if(m_checkStates.value(player, Qt::Unchecked)) {
            result.append(player);
        }
    }
    return result;
}

/*********************************************************************
 * DrinksSortFilterModel
 */
class LiveDrinkRoundModel : public QStandardItemModel
{
public:
    enum DataRoles {
        LiveDrinksRole = Qt::UserRole + 1,
        DrinkRole,
        PlayerRole
    };

    explicit LiveDrinkRoundModel(QObject *parent);

    void setRound(QSharedPointer<Round> round);
    void addLiveDrink(QSharedPointer<LiveDrink> liveDrink);
    void removeSelectedDrinks(const QModelIndexList &list);

private:
    QSharedPointer<Round> m_round;

    bool removeOneDrinkFromItem(QStandardItem *item, QStandardItem *parent);

    QStandardItem *findDrink(QSharedPointer<Drink> drink);
    QStandardItem *findPlayer(QStandardItem *drinkItem, QSharedPointer<Player> player);
    void addDrinkToItem(QStandardItem *playerItem, QSharedPointer<LiveDrink> liveDrink);
};

LiveDrinkRoundModel::LiveDrinkRoundModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

void LiveDrinkRoundModel::setRound(QSharedPointer<Round> round)
{
    clear();
    foreach(QSharedPointer<LiveDrink> liveDrink, round->drinks()) {
        addLiveDrink(liveDrink);
    }
    m_round = round;
}

void LiveDrinkRoundModel::addLiveDrink(QSharedPointer<LiveDrink> liveDrink)
{
    QSharedPointer<Drink> drink = liveDrink->drink();
    QSharedPointer<Player> player = liveDrink->player();

    QStandardItem *drinkItem = findDrink(drink);
    if(!drinkItem) {
        drinkItem = new QStandardItem(drink->name());
        invisibleRootItem()->appendRow(drinkItem);
        drinkItem->setData(QVariant::fromValue<QSharedPointer<Drink> >(drink), DrinkRole);
    }

    QStandardItem *playerItem = findPlayer(drinkItem, player);
    if(!playerItem) {
        playerItem = new QStandardItem;
        playerItem->setData(QVariant::fromValue<QSharedPointer<Player> >(player), PlayerRole);
        drinkItem->appendRow(playerItem);
    }

    addDrinkToItem(playerItem, liveDrink);
}

QStandardItem *LiveDrinkRoundModel::findDrink(QSharedPointer<Drink> drink)
{
    int childCount = invisibleRootItem()->rowCount();
    for(int i = 0; i < childCount; ++i) {
        QStandardItem *item = invisibleRootItem()->child(i);
        QSharedPointer<Drink> itemDrink = item->data(DrinkRole).value<QSharedPointer<Drink> >();
        if(itemDrink == drink)
            return item;
    }

    return nullptr;
}

QStandardItem *LiveDrinkRoundModel::findPlayer(QStandardItem *drinkItem, QSharedPointer<Player> player)
{
    int childCount = drinkItem->rowCount();
    for(int i = 0; i < childCount; ++i) {
        QStandardItem *item = drinkItem->child(i);
        QSharedPointer<Player> itemPlayer = item->data(PlayerRole).value<QSharedPointer<Player> >();
        if(itemPlayer == player)
            return item;
    }

    return nullptr;
}

void LiveDrinkRoundModel::addDrinkToItem(QStandardItem *playerItem, QSharedPointer<LiveDrink> liveDrink)
{
    QList<QSharedPointer<LiveDrink> > itemLiveDrinks = playerItem->data(LiveDrinksRole).value<QList<QSharedPointer<LiveDrink> > >();
    itemLiveDrinks.append(liveDrink);
    playerItem->setData(QVariant::fromValue<QList<QSharedPointer<LiveDrink> > >(itemLiveDrinks), LiveDrinksRole);

    QSharedPointer<Player> itemPlayer = playerItem->data(PlayerRole).value<QSharedPointer<Player> >();
    playerItem->setText(QString("%1x %2")
                        .arg(itemLiveDrinks.size())
                        .arg(itemPlayer->name()));
}

void LiveDrinkRoundModel::removeSelectedDrinks(const QModelIndexList &list)
{
    foreach(QModelIndex index, list) {
        QStandardItem *item = itemFromIndex(index);

        if(index.parent().isValid()) {
            QStandardItem *parent = item->parent();
            removeOneDrinkFromItem(item, item->parent());

            if(parent->rowCount() == 0) {
                invisibleRootItem()->removeRow(parent->row());
            }
            continue;
        }

        for(int i = 0; i < item->rowCount();) {
            if(!removeOneDrinkFromItem(item->child(i), item))
                ++i;
        }

        if(item->rowCount() == 0) {
            invisibleRootItem()->removeRow(item->row());
        }
    }
}

bool LiveDrinkRoundModel::removeOneDrinkFromItem(QStandardItem *playerItem, QStandardItem *drinkItem)
{
    QList<QSharedPointer<LiveDrink> > itemLiveDrinks = playerItem->data(LiveDrinksRole)
            .value<QList<QSharedPointer<LiveDrink> > >();
    QSharedPointer<LiveDrink> liveDrink = itemLiveDrinks.takeFirst();
    liveDrink->clearRelations();
    Qp::remove(liveDrink);

    if(itemLiveDrinks.isEmpty()) {
        drinkItem->removeRow(playerItem->row());
        return true;
    }

    playerItem->setData(QVariant::fromValue<QList<QSharedPointer<LiveDrink> > >(itemLiveDrinks), LiveDrinksRole);
    QSharedPointer<Player> itemPlayer = playerItem->data(PlayerRole).value<QSharedPointer<Player> >();
    playerItem->setText(QString("%1x %2")
                        .arg(itemLiveDrinks.size())
                        .arg(itemPlayer->name()));
    return false;
}


/*********************************************************************
 * DrinksWidget
 */
DrinksWidget::DrinksWidget(QSharedPointer<Round> round, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrinksWidget),
    m_round(round)
{
    ui->setupUi(this);
    setFocusProxy(ui->lineEditSearchDrink);

    ui->lineEditSearchDrink->installEventFilter(this);
    ui->listViewDrinks->installEventFilter(this);
    ui->listViewPlayers->installEventFilter(this);
    ui->treeViewDrinkRound->installEventFilter(this);

    ui->lineEditSearchDrink->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listViewDrinks->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listViewPlayers->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewDrinkRound->setAttribute(Qt::WA_MacShowFocusRect, false);

    DrinksListModel *drinksModel = new DrinksListModel(this);
    m_drinksModel = new DrinksSortFilterModel(drinksModel, this);
    m_drinksModel->sort(Qt::DescendingOrder);
    m_drinksModel->setFilterKeyColumn(1);
    m_drinksModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->listViewDrinks->setModel(m_drinksModel);
    ui->listViewDrinks->setModelColumn(DrinksListModel::NameColumn);

    PlayersListModel *playersModel = new PlayersListModel(this);
    playersModel->setObjects(round->game()->players());
    m_playersModel = new DrinkPlayersModel(playersModel, this);
    ui->listViewPlayers->setModel(m_playersModel);
    ui->listViewPlayers->setModelColumn(PlayersListModel::NameColumn);

    m_drinkRoundModel = new LiveDrinkRoundModel(this);
    m_drinkRoundModel->setRound(m_round);
    ui->treeViewDrinkRound->setModel(m_drinkRoundModel);
    ui->treeViewDrinkRound->expandAll();

    connect(drinksModel, &DrinksListModel::rowsInserted,
            this, &DrinksWidget::selectDrinkIfNoneSelected);
    connect(ui->listViewDrinks->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &DrinksWidget::onDrinkSelectionChanged);
}

DrinksWidget::~DrinksWidget()
{
    delete ui;
}

void DrinksWidget::on_lineEditSearchDrink_textChanged(const QString &arg1)
{
    m_drinksModel->setFilterWildcard(QString("*%1*").arg(arg1));

    if(arg1.isEmpty())
        m_playersModel->clearCheckStates();

    selectDrinkIfNoneSelected();
}

void DrinksWidget::showEvent(QShowEvent *)
{
    ui->lineEditSearchDrink->setFocus();

    selectDrinkIfNoneSelected();
}

bool DrinksWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        QWidget *focus = focusWidget();
        Qt::Key key = static_cast<Qt::Key>(keyEvent->key());

        switch(key) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if(focus == ui->listViewDrinks
                    || focus == ui->lineEditSearchDrink) {
                ui->listViewPlayers->setFocus();
            }
            else if(focus == ui->listViewPlayers) {
                addLiveDrinkForSelection();
            }
            break;

        case Qt::Key_Backspace:
            // From drinks and players lists, backspace edits the filter
            if(focus == ui->listViewDrinks
                    || ui->listViewPlayers) {
                ui->lineEditSearchDrink->setFocus();
            }
            // From drink round treeview, it removes the selected drink(s)
            if(focus == ui->treeViewDrinkRound) {
                removeSelectedDrinks();
                ui->treeViewDrinkRound->setFocus();
            }
            break;

        case Qt::Key_Up:
        case Qt::Key_Down:
            // From filter line edit, allow choosing a drink without pressing TAB
            if(focus == ui->lineEditSearchDrink) {
                ui->listViewDrinks->setFocus();
                QApplication::sendEvent(ui->listViewDrinks, event);
                return true;
            }
            break;


        case Qt::Key_Tab:
            // Prevent tabbing "out of the widget"
            if(focus == ui->treeViewDrinkRound) {
                return true;
            }
            break;
        case Qt::Key_Backtab:
            // Prevent tabbing "out of the widget"
            if(focus == ui->lineEditSearchDrink) {
                return true;
            }
            break;
        default:
            break;
        }

        // Redirect all keys, that "type something" to the filter line edit.
        // If this jumps to the filter (from somewhere else), clear it, so that you "start fresh".
        if(key >= Qt::Key_Exclam
                && key <= Qt::Key_Z) {
            if(focus != ui->lineEditSearchDrink) {
                ui->lineEditSearchDrink->clear();
                ui->lineEditSearchDrink->event(event); // Process the key
            }

            ui->lineEditSearchDrink->setFocus();
        }
    }

    // standard event processing
    return QObject::eventFilter(obj, event);
}

void DrinksWidget::selectDrinkIfNoneSelected()
{
    if(m_drinksModel->rowCount() <= 0)
        return;

    if(!ui->listViewDrinks->selectionModel()->selectedRows().isEmpty())
        return;

    ui->listViewDrinks->setCurrentIndex(ui->listViewDrinks->model()->index(0,0));
}

void DrinksWidget::onDrinkSelectionChanged(const QItemSelection &selected, const QItemSelection &)
{
    QSharedPointer<Drink> drink;
    if(!selected.indexes().isEmpty())
        drink = m_drinksModel->objectByIndex(selected.indexes().first());

    m_playersModel->setDrink(drink);

    if(m_playersModel->rowCount() > 0
            && ui->listViewPlayers->selectionModel()->selectedIndexes().isEmpty()) {
        ui->listViewPlayers->setCurrentIndex(ui->listViewPlayers->model()->index(0,0));
    }
}

void DrinksWidget::addLiveDrinkForSelection()
{
    QSharedPointer<Drink> drink = m_playersModel->drink();
    foreach(QSharedPointer<Player> player, m_playersModel->checkedPlayers()) {
        QSharedPointer<LiveDrink> liveDrink = Qp::create<LiveDrink>();
        liveDrink->setDrink(drink);
        liveDrink->setPlayer(player);
        liveDrink->setRound(m_round);
        Qp::update(liveDrink);
        m_drinkRoundModel->addLiveDrink(liveDrink);
    }

    ui->treeViewDrinkRound->expandAll();
}

void DrinksWidget::removeSelectedDrinks()
{
    QModelIndexList list = ui->treeViewDrinkRound->selectionModel()->selectedRows();

    m_drinkRoundModel->removeSelectedDrinks(list);
    ui->treeViewDrinkRound->expandAll();
}


void DrinksWidget::on_pushButtonAdd_clicked()
{
    addLiveDrinkForSelection();
}

void DrinksWidget::on_pushButtonRemove_clicked()
{
    removeSelectedDrinks();
}
