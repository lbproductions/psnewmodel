#include "statstreewidget.h"

#include <QHeaderView>

#include <data/league.h>
#include <data/player.h>
#include <data/playerstatistics.h>
#include <data/game.h>

#include <misc/tools.h>

StatsTreeWidget::StatsTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setSortingEnabled(true);

    this->setPalette(Tools::darkPalette(this));
    this->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
}

void StatsTreeWidget::setLeague(QSharedPointer<League> league)
{
    setGames(league->calculatedGames(), league->players());
}

void StatsTreeWidget::setGames(QList<QSharedPointer<Game> > games)
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

void StatsTreeWidget::highlightMax(Qt::SortOrder order, QColor backgroundColor)
{
    for(int column = 1; column < this->columnCount(); column++) {
        sortByColumn(column, order);
        for(int i = 0; i<topLevelItemCount(); i++) {
            if(topLevelItem(i)->data(column, Qt::DisplayRole) == topLevelItem(0)->data(column, Qt::DisplayRole)) {
                topLevelItem(i)->setBackgroundColor(column, backgroundColor);
            }
        }
    }
}

void StatsTreeWidget::setColumnsWidthToContent()
{
    for(int column = 0; column < this->columnCount(); column++) {
        resizeColumnToContents(column);
    }
}
