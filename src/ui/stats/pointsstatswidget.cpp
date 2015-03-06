#include "pointsstatswidget.h"

#include <QHeaderView>

#include <data/game.h>
#include <data/player.h>
#include <data/league.h>
#include <data/playerstatistics.h>

#include "barchartwidget.h"

#include <misc/tools.h>

/*
PointsStatsWidget::PointsStatsWidget(QWidget *parent) :
    StatsTreeWidget(parent)
{
    setColumnCount(6);
    setHeaderLabels(QStringList() << tr("Player") << tr("RoundWins") << tr("AverageWinPoints") << tr("AverageReWinPoints") << tr("AverageContraWinPoints") << tr("TotalPoints"));
}

void PointsStatsWidget::setGames(QList<QSharedPointer<Game> > list, QList<QSharedPointer<Player> > players)
{
    foreach(QSharedPointer<Player> player, players) {
        PlayerStatistics* stats = new PlayerStatistics(this);
        stats->setPlayer(player.data());
        QList<QSharedPointer<Game> > playerGames;
        foreach(QSharedPointer<Game> game, list) {
            if(game->players().contains(player)) {
                playerGames.append(game);
            }
        }
        stats->setGames(playerGames);
        stats->setPlayer(player.data());

        QTreeWidgetItem* item = new QTreeWidgetItem(this);
        item->setText(0, player->name());
        item->setIcon(0, QIcon(player->colorPixmap()));
        item->setData(1, Qt::DisplayRole, stats->winRounds().size());
        item->setData(2, Qt::DisplayRole, stats->averagePointsPerRound());
        item->setData(3, Qt::DisplayRole, stats->averageRePointsPerRound());
        item->setData(4, Qt::DisplayRole, stats->averageContraPointsPerRound());
        item->setData(5, Qt::DisplayRole, stats->gamePoints());
        this->addTopLevelItem(item);
    }

    sortByColumn(0);

    highlightMax(Qt::DescendingOrder, QColor(0,123,0));
}
*/


PointsStatsWidget::PointsStatsWidget(QWidget *parent):
    QWidget(parent)
{
    m_layout = new QGridLayout(this);
    this->setLayout(m_layout);
}

void PointsStatsWidget::setLeague(QSharedPointer<League> league)
{
    setGames(league->calculatedGames(), league->players());
}

void PointsStatsWidget::setGames(QList<QSharedPointer<Game> > games)
{
    QList<QSharedPointer<Player> > players;
    foreach(QSharedPointer<Game> game, games) {
        foreach(QSharedPointer<Player> player, game->players()) {
            if(!players.contains(player)) {
                players.append(player);
            }
        }
    }

    setGames(games, players);
}

void PointsStatsWidget::setGames(QList<QSharedPointer<Game> > list, QList<QSharedPointer<Player> > players)
{
    QHash<QSharedPointer<Player>,PlayerStatistics*> statsHash;
    foreach(QSharedPointer<Player> player, players) {
        PlayerStatistics* stats = new PlayerStatistics(this);
        stats->setPlayer(player.data());
        QList<QSharedPointer<Game> > playerGames;
        foreach(QSharedPointer<Game> game, list) {
            if(game->players().contains(player)) {
                playerGames.append(game);
            }
        }
        stats->setGames(playerGames);
        stats->setPlayer(player.data());

        statsHash.insert(player, stats);
        /*
        QTreeWidgetItem* item = new QTreeWidgetItem(this);
        item->setText(0, player->name());
        item->setIcon(0, QIcon(player->colorPixmap()));
        item->setData(1, Qt::DisplayRole, stats->winRounds().size());
        item->setData(2, Qt::DisplayRole, stats->averagePointsPerRound());
        item->setData(3, Qt::DisplayRole, stats->averageRePointsPerRound());
        item->setData(4, Qt::DisplayRole, stats->averageContraPointsPerRound());
        item->setData(5, Qt::DisplayRole, stats->gamePoints());
        this->addTopLevelItem(item);
        */
    }

    BarChartWidget* barCharts = createBarWidget();
    barCharts->setTitle("WinRounds");
    foreach(QSharedPointer<Player> player, players) {
        barCharts->setValue(player, statsHash.value(player)->winRounds().size());
    }
    m_layout->addWidget(barCharts, 0,0);

    barCharts = createBarWidget();
    barCharts->setTitle("WinRounds/Game");
    foreach(QSharedPointer<Player> player, players) {
        double value = 0;
        if(statsHash.value(player)->games().size() > 0) {
            value = (double)statsHash.value(player)->winRounds().size() / (double)statsHash.value(player)->games().size();
        }
        barCharts->setValue(player, value);
    }
    m_layout->addWidget(barCharts, 0,1);

    barCharts = createBarWidget();
    barCharts->setTitle("WinPoints/Round");
    foreach(QSharedPointer<Player> player, players) {
        barCharts->setValue(player, statsHash.value(player)->averagePointsPerRound());
    }
    m_layout->addWidget(barCharts, 1,0);

    barCharts = createBarWidget();
    barCharts->setTitle("WinPoints/ReRound");
    foreach(QSharedPointer<Player> player, players) {
        barCharts->setValue(player, statsHash.value(player)->averageRePointsPerRound());
    }
    m_layout->addWidget(barCharts, 2,0);

    barCharts = createBarWidget();
    barCharts->setTitle("WinPoints/ContraRound");
    foreach(QSharedPointer<Player> player, players) {
        barCharts->setValue(player, statsHash.value(player)->averageContraPointsPerRound());
    }
    m_layout->addWidget(barCharts, 2,1);
}

BarChartWidget *PointsStatsWidget::createBarWidget()
{
    BarChartWidget* barCharts = new BarChartWidget(this);
    barCharts->setSortingOrder(BarChartWidget::Descending);
    barCharts->setBarDistance(15);
    barCharts->setBarWidth(80);
    barCharts->setPadding(20,20,20,20);
    barCharts->setValueFormatString("%1");
    this->layout()->addWidget(barCharts);
    return barCharts;
}
