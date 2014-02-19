#include "startwidget.h"
#include "ui_startwidget.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "ui/model/gamelistmodel.h"
#include "noisygradientitemdelegate.h"
#include "ui/game/gamewindow.h"

#include <data/player.h>

#include <QPainter>

/******************************************************************
 * UnfinishedGamesDelegate
 */
class UnfinishedGamesDelegate : public NoisyGradientItemDelegate
{
public:
    explicit UnfinishedGamesDelegate(QAbstractItemView *view, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<Game> forIndex(const QModelIndex &index) const;
};

UnfinishedGamesDelegate::UnfinishedGamesDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
}

void UnfinishedGamesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    QSharedPointer<Game> game = forIndex(index);
    if(!game)
        return;

    drawTitleText(painter, option,
                                         game->creationTime().toString(Qt::SystemLocaleDate),
                                         QPoint(7,7)).topRight();

    QPoint colorPos = option.rect.bottomLeft() + QPoint(7, -23);
    foreach(QSharedPointer<Player> player, game->players()) {
        painter->setBrush(player->color());

        if(player->color() == QColor(Qt::white))
            painter->setPen(Qt::gray);
        else
            painter->setPen(player->color());

        painter->drawRect(QRect(colorPos, QSize(16,16)));
        colorPos += QPoint(18,0);
    }

    drawText(painter, option,
             QString("%1 / %2 rounds")
             .arg(game->finishedRoundCount())
             .arg(game->totalRoundCount()),
             QPoint(0, 0),
             Qt::AlignRight | Qt::AlignVCenter);
}

QSize UnfinishedGamesDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = NoisyGradientItemDelegate::sizeHint(option, index);
    size.setHeight(size.height() + 40);
    return size;
}

QSharedPointer<Game> UnfinishedGamesDelegate::forIndex(const QModelIndex &index) const
{
    return static_cast<GameSortFilterModel *>(view()->model())->objectByIndex(index);
}


/******************************************************************
 * StartWidget
 */
StartWidget::StartWidget(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget)
{
    ui->setupUi(this);


    GameSortFilterModel *model = new GameSortFilterModel(new GameListModel(this), this);
    model->setFilterRole(GameSortFilterModel::UnfinishedStateFilter);
    model->setSortRole(GameSortFilterModel::Date);
    model->sort(0, Qt::DescendingOrder);

    ui->listViewGames->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listViewGames->setModel(model);
    ui->listViewGames->setItemDelegate(new UnfinishedGamesDelegate(ui->listViewGames, this));
}

StartWidget::~StartWidget()
{
    delete ui;
}

void StartWidget::init(MainWindow *window)
{
    ui->toolButtonStartGame->setDefaultAction(window->ui->actionNew_Game);
}

void StartWidget::on_listViewGames_doubleClicked(const QModelIndex &index)
{
    QSharedPointer<Game> game = static_cast<GameSortFilterModel *>(ui->listViewGames->model())->objectByIndex(index);

    if(!game)
        return;

    GameWindow *window = new GameWindow;
    window->setGame(game);
    window->show();
}
