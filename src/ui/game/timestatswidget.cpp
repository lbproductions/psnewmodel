#include "timestatswidget.h"

#include <QHeaderView>

#include <data/league.h>
#include <data/player.h>
#include <data/playerstatistics.h>
#include <data/game.h>

#include <misc/tools.h>

TimeStatsWidget::TimeStatsWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setColumnCount(3);
    setHeaderLabels(QStringList() << tr("Player") << tr("AverageRoundTime") << tr("AverageSoloTime"));
    setSortingEnabled(true);

    this->setPalette(Tools::darkPalette(this));
    this->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
}

void TimeStatsWidget::setLeague(QSharedPointer<League> league)
{
    foreach(QSharedPointer<Player> player, league->players()) {
        this->addTopLevelItem(
                    createItem(player,
                               league->playerStats(player)->averageRoundTime(),
                               league->playerStats(player)->averageSoloTime())
                    );
    }
}

void TimeStatsWidget::setGames(QList<QSharedPointer<Game> > games)
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

void TimeStatsWidget::setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players)
{
    foreach(QSharedPointer<Player> player, players) {
        PlayerStatistics* stats = new PlayerStatistics(this);
        stats->setPlayer(player.data());
        QList<QSharedPointer<Game> > playerGames;
        foreach(QSharedPointer<Game> game, games) {
            if(game->players().contains(player)) {
                playerGames.append(game);
            }
        }
        stats->setGames(playerGames);

        this->addTopLevelItem(
                    createItem(player,
                               stats->averageRoundTime(),
                               stats->averageSoloTime())
                    );

        delete stats;
        stats = 0;
    }
}

QTreeWidgetItem *TimeStatsWidget::createItem(QSharedPointer<Player> player, QTime averageTime, QTime averageSoloTime)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this);
    item->setText(0, player->name());
    item->setIcon(0, QIcon(player->colorPixmap()));
    item->setData(1, Qt::DisplayRole, averageTime.toString("mm:ss"));
    item->setData(2, Qt::DisplayRole, averageSoloTime.toString("mm:ss"));
    return item;
}
