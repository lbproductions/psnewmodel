#ifndef TIMESTATSWIDGET_H
#define TIMESTATSWIDGET_H

#include <QTreeWidget>

class League;
class Game;
class Player;

class TimeStatsWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit TimeStatsWidget(QWidget *parent = 0);

    void setLeague(QSharedPointer<League> league);

    void setGames(QList<QSharedPointer<Game> > games);
    void setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players);

private:
    QTreeWidgetItem* createItem(QSharedPointer<Player> player, QTime averageTime, QTime averageSoloTime);

};

#endif // TIMESTATSWIDGET_H
