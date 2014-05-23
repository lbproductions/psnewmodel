#include "pointsstatswidget.h"

#include <QHeaderView>

#include <data/game.h>
#include <data/player.h>
#include <data/league.h>
#include <data/playerstatistics.h>

#include <misc/tools.h>

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
