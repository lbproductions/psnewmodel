#ifndef SERIESSTATSWIDGET_H
#define SERIESSTATSWIDGET_H

#include <QTreeWidget>
#include <data/playerstatistics.h>

class Game;
class Player;
class League;
class PlayerStatistics;

class SeriesStatsWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SeriesStatsWidget(QWidget *parent = 0);

    void setLeague(QSharedPointer<League> league);

    void setGames(QList<QSharedPointer<Game> > games);
    void setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players);

    void createItemFromPlayerStats(QSharedPointer<PlayerStatistics> stats);

    QTreeWidgetItem* createItem(QSharedPointer<Player> player, QString wins, QString losses, QString current);

signals:

public slots:

private:
    int seriesPoints(RoundSeries series, QSharedPointer<Player> player);

};

#endif // SERIESSTATSWIDGET_H
