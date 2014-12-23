#include "seriesstatswidget.h"

#include <data/league.h>
#include <data/player.h>
#include <data/game.h>
#include <data/round.h>

#include <misc/tools.h>
#include <QHeaderView>

SeriesStatsWidget::SeriesStatsWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setColumnCount(4);
    setHeaderLabels(QStringList() << tr("Player") << tr("Longest Win Series") << tr("Longest Lose Series") << tr("Current Series"));
    setSortingEnabled(true);

    this->setPalette(Tools::darkPalette(this));
    this->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
}

void SeriesStatsWidget::setLeague(QSharedPointer<League> league)
{
    foreach(QSharedPointer<Player> player, league->players()) {
        createItemFromPlayerStats(league->playerStats(player));
    }
}

void SeriesStatsWidget::setGames(QList<QSharedPointer<Game> > games)
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

void SeriesStatsWidget::setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players)
{
    if(games.isEmpty())
        return;

    if(games.size() == 1 && games.first()->rounds().size() == 1) {
        return;
    }

    foreach(QSharedPointer<Player> player, players) {
        QSharedPointer<PlayerStatistics> stats = QSharedPointer<PlayerStatistics>(new PlayerStatistics(this));
        stats->setPlayer(player.data());
        QList<QSharedPointer<Game> > playerGames;
        foreach(QSharedPointer<Game> game, games) {
            if(game->players().contains(player)) {
                playerGames.append(game);
            }
        }
        stats->setGames(playerGames);

        createItemFromPlayerStats(stats);
    }
}

void SeriesStatsWidget::createItemFromPlayerStats(QSharedPointer<PlayerStatistics> stats)
{
    QSharedPointer<Player> player = stats->player();

    QString best = "-";
    QList<RoundSeries> bests = stats->winSeries();
    if(!bests.isEmpty()) {
        RoundSeries bestSeries = bests.first();
        best = "W" + QString::number(bestSeries.size()) + " (" + QString::number(seriesPoints(bestSeries, player)) + ")";
    }

    QString worst = "-";
    QList<RoundSeries> worsts = stats->loseSeries();
    if(!worsts.isEmpty()) {
        RoundSeries worstSeries = worsts.first();
        worst = "L" + QString::number(worstSeries.size()) + " (" + QString::number(seriesPoints(worstSeries, player)) + ")";
    }

    QString current = "-";
    RoundSeries currentSeries = stats->lastSeries();
    if(!currentSeries.isEmpty()) {
        if(currentSeries.first()->points(player) > 0) {
            current = "W";
        }
        else if(currentSeries.first()->points(player) < 0) {
            current = "L";
        }
        else {
            current = "U";
        }
        current += QString::number(currentSeries.size()) + " (" + QString::number(seriesPoints(currentSeries, player)) + ")";
    }

    this->addTopLevelItem(
                createItem(player,
                           best,
                           worst,
                           current)
                );
}

QTreeWidgetItem *SeriesStatsWidget::createItem(QSharedPointer<Player> player, QString wins, QString losses, QString current)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this);
    item->setText(0, player->name());
    item->setIcon(0, QIcon(player->colorPixmap()));
    item->setText(1, wins);
    item->setText(2, losses);
    item->setText(3, current);
    return item;
}

int SeriesStatsWidget::seriesPoints(RoundSeries series, QSharedPointer<Player> player)
{
    int points = 0;

    foreach(QSharedPointer<Round> round, series) {
        points += round->points(player);
    }

    return points;
}
