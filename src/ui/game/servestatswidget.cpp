#include "servestatswidget.h"

#include <data/league.h>
#include <data/player.h>
#include <data/playerstatistics.h>
#include <data/game.h>

#include <misc/tools.h>
#include <QHeaderView>

ServeStatsWidget::ServeStatsWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setColumnCount(3);
    setHeaderLabels(QStringList() << tr("Player") << tr("AbsLostServe") << tr("RelLostServe"));
    setSortingEnabled(true);

    this->setPalette(Tools::darkPalette(this));
    this->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
}

void ServeStatsWidget::setLeague(QSharedPointer<League> league)
{
    setGames(league->calculatedGames(), league->players());
}

void ServeStatsWidget::setGames(QList<QSharedPointer<Game> > games)
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

void ServeStatsWidget::setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players)
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

        double relative = 0;
        if(stats->serveRounds().size() > 0) {
            relative = (double)stats->lostServe() / (double)stats->serveRounds().size();
        }


        this->addTopLevelItem(
                    createItem(player,
                               stats->lostServe(),
                               relative)
                    );

        delete stats;
        stats = 0;
    }
}

QTreeWidgetItem *ServeStatsWidget::createItem(QSharedPointer<Player> player, int absLostServe, double relLostServe)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this);
    item->setText(0, player->name());
    item->setIcon(0, QIcon(player->colorPixmap()));
    item->setData(1, Qt::DisplayRole, absLostServe);
    item->setData(2, Qt::DisplayRole, relLostServe);
    return item;
}
