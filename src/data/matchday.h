#ifndef MATCHDAY_H
#define MATCHDAY_H

#include <QObject>
#include <QSharedPointer>

class League;
class Game;
class Player;

class Matchday : public QObject
{
        Q_OBJECT
    public:
        explicit Matchday(QObject *parent = 0);
        ~Matchday();

        void setGame(QSharedPointer<Game> game);
        QSharedPointer<Game> game();

        QList<QSharedPointer<Player> > playersSortedByAverage();
        void setPlayersSortedByAverage(QList<QSharedPointer<Player> > list);
        void addPlayer(QSharedPointer<Player> player);

        double average(QSharedPointer<Player> player);
        void addAverage(QSharedPointer<Player> player, double average);

        int placementOfPlayer(QSharedPointer<Player> player);

        void setNumber(int number);
        int number();
        
    signals:
        
    public slots:

    private:
        QSharedPointer<Game> m_game;

        QList<QSharedPointer<Player> > m_playersSortedByAverage;
        QHash<QSharedPointer<Player>, double> m_averages;

        int m_number;

};

#endif // MATCHDAY_H
