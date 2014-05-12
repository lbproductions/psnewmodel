#ifndef POINTSSTATSWIDGET_H
#define POINTSSTATSWIDGET_H

#include <QTreeWidget>

class Game;
class PlayerStatistics;
class Player;
class League;

class PointsStatsWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PointsStatsWidget(QWidget *parent = 0);

    void setGame(QSharedPointer<Game> game);
    void setGames(QList<QSharedPointer<Game> > games);
    void setGames(QList<QSharedPointer<Game> > list, QList<QSharedPointer<Player> > players);
    void setLeague(QSharedPointer<League> league);

signals:

public slots:

private:
    QList<QSharedPointer<Game> > m_games;

    QHash<QSharedPointer<Player>, PlayerStatistics*> m_playerStats;

};

#endif // POINTSSTATSWIDGET_H
