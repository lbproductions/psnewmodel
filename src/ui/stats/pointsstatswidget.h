#ifndef POINTSSTATSWIDGET_H
#define POINTSSTATSWIDGET_H

#include "statstreewidget.h"

#include <QGridLayout>

class Game;
class PlayerStatistics;
class Player;
class BarChartWidget;

//class PointsStatsWidget : public StatsTreeWidget
class PointsStatsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PointsStatsWidget(QWidget *parent = 0);

    void setLeague(QSharedPointer<League> league);

    void setGames(QList<QSharedPointer<Game> > games);

    void setGames(QList<QSharedPointer<Game> > list, QList<QSharedPointer<Player> > players);

private:
    BarChartWidget* createBarWidget();

    QGridLayout* m_layout;
};

#endif // POINTSSTATSWIDGET_H
