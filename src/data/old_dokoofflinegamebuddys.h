#ifndef OLD_DOKOOFFLINEGAMEBUDDYS_H
#define OLD_DOKOOFFLINEGAMEBUDDYS_H

#include <QObject>
#include <QPersistenceRelations.h>

class Game;
class Player;

class OLD_DokoOfflineGameBuddys : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QSharedPointer<Game> game READ game WRITE setGame)
        Q_PROPERTY(QSharedPointer<Player> player1 READ player1 WRITE setPlayer1)
        Q_PROPERTY(QSharedPointer<Player> player2 READ player2 WRITE setPlayer2)
        Q_PROPERTY(int rounds READ rounds WRITE setRounds)
        Q_PROPERTY(int wins READ wins WRITE setWins)

        Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:game",
                    "reverserelation=dokoOfflineGameBuddys")

    public:
        OLD_DokoOfflineGameBuddys(QObject *parent = 0);

        QSharedPointer<Game> game() const;
        void setGame(QSharedPointer<Game> game);

        QSharedPointer<Player> player1() const;
        void setPlayer1(QSharedPointer<Player> player1);

        QSharedPointer<Player> player2() const;
        void setPlayer2(QSharedPointer<Player> player2);

        int rounds() const;
        void setRounds(const int rounds);

        int wins() const;
        void setWins(const int wins);

    private:
        QpWeakRelation<Game> m_game;
        QpWeakRelation<Player> m_player1;
        QpWeakRelation<Player> m_player2;

        int m_rounds;
        int m_wins;
};

#endif // OLD_DOKOOFFLINEGAMEBUDDYS_H
