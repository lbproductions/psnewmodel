#ifndef SERVESTATSWIDGET_H
#define SERVESTATSWIDGET_H

#include <QTreeWidget>

class Game;
class Player;
class League;

class ServeStatsWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ServeStatsWidget(QWidget *parent = 0);

    void setLeague(QSharedPointer<League> league);

    void setGames(QList<QSharedPointer<Game> > games);
    void setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players);

    QTreeWidgetItem* createItem(QSharedPointer<Player> player, int absLostServe, double relLostServe);
signals:

public slots:

};

#endif // SERVESTATSWIDGET_H
