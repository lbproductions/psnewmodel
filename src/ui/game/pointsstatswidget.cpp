#include "pointsstatswidget.h"

#include <QHeaderView>

#include <data/game.h>
#include <data/player.h>
#include <data/playerstatistics.h>

#include <misc/tools.h>

PointsStatsWidget::PointsStatsWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setColumnCount(6);
    setHeaderLabels(QStringList() << tr("Player") << tr("RoundWins") << tr("AverageWinPoints") << tr("AverageReWinPoints") << tr("AverageContraWinPoints") << tr("TotalPoints"));
    setSortingEnabled(true);

    this->setPalette(Tools::darkPalette(this));
    this->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
}

void PointsStatsWidget::setGame(QSharedPointer<Game> game)
{
    setGames(QList<QSharedPointer<Game> >() << game);
}

void PointsStatsWidget::setGames(QList<QSharedPointer<Game> > games)
{
    m_games = games;

    foreach(QSharedPointer<Game> game, m_games) {

        foreach(QSharedPointer<Player> player, game->players()) {
            if(!m_playerStats.contains(player)) {
                PlayerStatistics* stats = new PlayerStatistics(this);
                stats->setPlayer(player.data());
                stats->setGames(m_games);
                m_playerStats.insert(player, stats);

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
        }
    }

    sortByColumn(0);

    for(int column = 1; column < this->columnCount(); column++) {
        sortByColumn(column);
        for(int i = 0; i<topLevelItemCount(); i++) {
            if(topLevelItem(i)->data(column, Qt::DisplayRole).toDouble() == topLevelItem(0)->data(column, Qt::DisplayRole).toDouble()) {
                topLevelItem(i)->setBackgroundColor(column, QColor(0, 123, 0));
            }
        }
    }
}
