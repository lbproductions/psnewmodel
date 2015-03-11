#include "startwidget.h"
#include "ui_startwidget.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "ui/model/gamelistmodel.h"
#include "ui/game/gamewindow.h"

#include <ui/delegates/leaguesdelegate.h>
#include <ui/delegates/gamesdelegate.h>

#include "ui/model/leaguelistmodel.h"
#include "ui/league/leaguewindow.h"

#include <data/place.h>
#include <data/player.h>

#include <QPainter>


/******************************************************************
 * StartWidget
 */
StartWidget::StartWidget(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget)
{
    ui->setupUi(this);


    m_sortModel = new GameSortFilterModel(new GameListModel(this), this);
    m_sortModel->setFilterRole(GameSortFilterModel::AllGamesStateFilter);
    m_sortModel->setSortRole(GameSortFilterModel::Date);
    m_sortModel->sort(0, Qt::DescendingOrder);

    ui->listViewGames->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listViewGames->setModel(m_sortModel);
    ui->listViewGames->setItemDelegate(new GamesDelegate(ui->listViewGames, this));

    LeagueSortFilterModel *leagueModel = new LeagueSortFilterModel(new LeagueListModel(this), this);
    leagueModel->setSortRole(LeagueSortFilterModel::Name);
    leagueModel->sort(0, Qt::DescendingOrder);

    ui->listViewLeague->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listViewLeague->setModel(leagueModel);
    ui->listViewLeague->setItemDelegate(new LeaguesDelegate(ui->listViewLeague, this));

    ui->checkBox->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEditSearch->setAttribute(Qt::WA_MacShowFocusRect, false);
}

StartWidget::~StartWidget()
{
    delete ui;
}

void StartWidget::init(MainWindow *window)
{
    ui->toolButtonStartGame->setDefaultAction(window->ui->actionNew_Game);
    ui->toolButtonNewLeague->setDefaultAction(window->ui->actionNew_League);
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

void StartWidget::on_listViewLeague_doubleClicked(const QModelIndex &index)
{
    QSharedPointer<League> league = static_cast<LeagueSortFilterModel *>(ui->listViewLeague->model())->objectByIndex(index);

    if(!league)
        return;

    LeagueWindow *window = new LeagueWindow;
    window->setLeague(league);
    window->show();
}

void StartWidget::on_lineEditSearch_textChanged(const QString &arg1)
{
    if(arg1 == "") {
        m_sortModel->setPlayers(QStringList());
        return;
    }

    QString text = ui->lineEditSearch->text();
    text.remove(" ");
    QStringList items = text.split(",");

    m_sortModel->setPlayers(items);
}

void StartWidget::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()) {
        m_sortModel->setFilterRole(GameSortFilterModel::UnfinishedStateFilter);
    }
    else {
       m_sortModel->setFilterRole(GameSortFilterModel::AllGamesStateFilter);
    }
}
