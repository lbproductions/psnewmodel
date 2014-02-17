#include "drinkswidget.h"
#include "ui_drinkswidget.h"

#include "data/player.h"
#include "data/drink.h"
#include "ui/model/drinkslistmodel.h"
#include "ui/model/playerslistmodel.h"
#include "misc/tools.h"
#include "data/game.h"
#include "data/livedrink.h"
#include "data/round.h"

#include <QPersistence.h>

#include <QKeyEvent>
#include <QLabel>
#include <QStandardItemModel>
#include <QScrollBar>

/*********************************************************************
 * DrinksSortFilterModel
 */
class DrinksSortFilterModel : public QpSortFilterProxyObjectModel<Drink>
{
public:
    explicit DrinksSortFilterModel(QpObjectListModel<Drink> *sourceModel, QObject *parent = 0);

    QSharedPointer<Round> round() const;
    void setRound(const QSharedPointer<Round> &round);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
    bool lessThan(QSharedPointer<Drink> left, QSharedPointer<Drink> right) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<Round> m_round;
    QSharedPointer<Game> m_game;
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
    if(!m_game)
        return left->count() > right->count();

    int leftCount = m_game->drinkCount(left);
    int rightCount = m_game->drinkCount(right);

    if(leftCount != rightCount)
        return leftCount > rightCount;

    return left->count() > right->count();
}

QSharedPointer<Round> DrinksSortFilterModel::round() const
{
    return m_round;
}

void DrinksSortFilterModel::setRound(const QSharedPointer<Round> &round)
{
    m_game = round->game();
    m_round = round;
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

    QSharedPointer<Round> round() const;
    void setRound(const QSharedPointer<Round> &round);

protected:
    bool lessThan(QSharedPointer<Player> left, QSharedPointer<Player> right) const Q_DECL_OVERRIDE;

    mutable QHash<QSharedPointer<Player>, Qt::CheckState> m_checkStates;
    QSharedPointer<Drink> m_drink;
    QSharedPointer<Round> m_round;
    QSharedPointer<Game> m_game;
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
        if(!m_game || !m_drink)
            return Qt::Unchecked;

        QSharedPointer<Player> player = objectByIndex(index);
        if(m_checkStates.contains(player))
            return m_checkStates.value(player);

        int count = m_game->drinkCounts(player).value(m_drink);
        if(count == 0)
            return Qt::Unchecked;

        m_checkStates.insert(player, Qt::Checked);
        return Qt::Checked;
    }
    else if(role == Qt::DecorationRole) {
        if(!m_game || !m_drink)
            return QVariant();

        QSharedPointer<Player> player = objectByIndex(index);
        return player->colorPixmap();
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
    if(!m_game || !m_drink)
        return QpSortFilterProxyObjectModel<Player>::lessThan(left, right);

    int leftCount = m_game->drinkCounts(left).value(m_drink);
    int rightCount = m_game->drinkCounts(right).value(m_drink);

    if(leftCount != rightCount)
        return leftCount > rightCount;

    return m_drink->count(left) > m_drink->count(right);
}

QSharedPointer<Round> DrinkPlayersModel::round() const
{
    return m_round;
}

void DrinkPlayersModel::setRound(const QSharedPointer<Round> &round)
{
    beginResetModel();
    m_round = round;
    m_game = round->game();
    m_checkStates.clear();
    endResetModel();
}


QSharedPointer<Drink> DrinkPlayersModel::drink() const
{
    return m_drink;
}

void DrinkPlayersModel::setDrink(const QSharedPointer<Drink> &drink)
{
    beginResetModel();
    m_drink = drink;
    m_checkStates.clear();
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
    foreach(QSharedPointer<LiveDrink> liveDrink, round->liveDrinks()) {
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
DrinksWidget::DrinksWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrinksWidget)
{
    ui->setupUi(this);
    setFocusProxy(ui->lineEditSearchDrink);
    setAttribute(Qt::WA_DeleteOnClose, true);

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
    m_playersModel = new DrinkPlayersModel(playersModel, this);
    m_playersModel->sort(Qt::DescendingOrder);
    ui->listViewPlayers->setModel(m_playersModel);
    ui->listViewPlayers->setModelColumn(PlayersListModel::NameColumn);

    m_drinkRoundModel = new LiveDrinkRoundModel(this);
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

    QModelIndex i = ui->listViewDrinks->model()->index(0, ui->listViewDrinks->modelColumn());
    ui->listViewDrinks->setCurrentIndex(i);
}

void DrinksWidget::onDrinkSelectionChanged(const QItemSelection &selected, const QItemSelection &)
{
    QSharedPointer<Drink> drink;
    if(!selected.indexes().isEmpty())
        drink = m_drinksModel->objectByIndex(selected.indexes().first());

    m_playersModel->setDrink(drink);

    QItemSelectionModel *selectionModel = ui->listViewPlayers->selectionModel();
    if(m_playersModel->rowCount() == 0
            || !selectionModel->selectedIndexes().isEmpty())
        return;

    QModelIndex i = ui->listViewPlayers->model()->index(0, ui->listViewPlayers->modelColumn());
    ui->listViewPlayers->setCurrentIndex(i);
}

void DrinksWidget::addLiveDrinkForSelection()
{
    Q_ASSERT(m_round);

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
    showExistingDrinks();
}

void DrinksWidget::removeSelectedDrinks()
{
    QModelIndexList list = ui->treeViewDrinkRound->selectionModel()->selectedRows();

    m_drinkRoundModel->removeSelectedDrinks(list);
    ui->treeViewDrinkRound->expandAll();
    showExistingDrinks();
}


void DrinksWidget::on_pushButtonAdd_clicked()
{
    addLiveDrinkForSelection();
}

void DrinksWidget::on_pushButtonRemove_clicked()
{
    removeSelectedDrinks();
}

QSharedPointer<Round> DrinksWidget::round() const
{
    return m_round;
}

void DrinksWidget::setRound(const QSharedPointer<Round> &round)
{
    m_round = round;
    m_drinkRoundModel->setRound(m_round);
    m_drinksModel->setRound(m_round);
    m_playersModel->sourceModel()->setObjects(m_round->game()->players());
    m_playersModel->setRound(m_round);
    ui->treeViewDrinkRound->expandAll();
    showExistingDrinks();
}

void DrinksWidget::showExistingDrinks()
{
    QWidget *widget = new QWidget(ui->stackedWidgetExistingDrinks);
    widget->setLayout(new QVBoxLayout);

    foreach(QSharedPointer<Player> player, m_round->game()->players()) {
        showExistingDrinksForPlayer(player, widget);
    }

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    widget->layout()->addWidget(spacer);

    delete ui->stackedWidgetExistingDrinks->currentWidget();
    ui->stackedWidgetExistingDrinks->addWidget(widget);
    ui->stackedWidgetExistingDrinks->setCurrentWidget(widget);
}

void DrinksWidget::showExistingDrinksForPlayer(QSharedPointer<Player> player, QWidget *widget)
{
    QLabel *labelName = new QLabel(player->name(), this);
    QLabel *labelPixmap = new QLabel(this);
    labelPixmap->setPixmap(player->colorPixmap());
    QLabel *labelDrinks = new QLabel(this);

    QMap<QSharedPointer<Drink>, int> drinkCounts = m_round->game()->drinkCounts(player);

    QStringList drinks;

    auto end = drinkCounts.end();
    for(auto it = drinkCounts.begin(); it != end; it++) {
        drinks.append(QString("%1x %2")
                      .arg(it.value())
                      .arg(it.key()->name()));
    }

    QString text = drinks.join("\n");
    if(text.isEmpty())
        text = "-";
    labelDrinks->setText(text);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(labelPixmap, 0);
    headerLayout->addWidget(labelName, 1);
    layout->addLayout(headerLayout);
    layout->addWidget(labelDrinks);
    static_cast<QVBoxLayout *>(widget->layout())->addLayout(layout);
}

