#ifndef POINTSSTATSWIDGET_H
#define POINTSSTATSWIDGET_H

#include "statstreewidget.h"

class Game;
class PlayerStatistics;
class Player;

class PointsStatsWidget : public StatsTreeWidget
{
    Q_OBJECT
public:
    explicit PointsStatsWidget(QWidget *parent = 0);

    void setGames(QList<QSharedPointer<Game> > list, QList<QSharedPointer<Player> > players);
};

#endif // POINTSSTATSWIDGET_H
