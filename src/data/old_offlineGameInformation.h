#ifndef DOKOOFFLINEGAME_H
#define DOKOOFFLINEGAME_H

#include <QObject>
#include <QPersistence.h>

class Game;
class Player;

class OLD_OfflineGameInformation: public QObject
{
        Q_OBJECT
        Q_PROPERTY(QSharedPointer<Game> game READ game WRITE setGame)
        Q_PROPERTY(QSharedPointer<Player> player READ player WRITE setPlayer)
        Q_PROPERTY(int hochzeiten READ hochzeiten WRITE setHochzeiten)
        Q_PROPERTY(int soli READ soli WRITE setSoli)
        Q_PROPERTY(int trumpfabgaben READ trumpfabgaben WRITE setTrumpfabgaben)
        Q_PROPERTY(int reRounds READ reRounds WRITE setReRounds)
        Q_PROPERTY(int reWins READ reWins WRITE setReWins)
        Q_PROPERTY(int contraRounds READ contraRounds WRITE setContraRounds)
        Q_PROPERTY(int contraWins READ contraWins WRITE setContraWins)
        Q_PROPERTY(int points READ points WRITE setPoints)
        Q_PROPERTY(int placement READ placement WRITE setPlacement)

        Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:player",
                    "reverserelation=offlineGameInformation")
        Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:game",
                    "reverserelation=offlineGameInformation")

    public:
        OLD_OfflineGameInformation(QObject *parent = 0);

        QSharedPointer<Game> game() const;
        void setGame(QSharedPointer<Game> game);

        QSharedPointer<Player> player() const;
        void setPlayer(QSharedPointer<Player> player);

        int hochzeiten() const;
        void setHochzeiten(const int hochzeiten);

        int soli() const;
        void setSoli(const int soli);

        int trumpfabgaben() const;
        void setTrumpfabgaben(const int trumpfabgaben);

        int reRounds() const;
        void setReRounds(const int reRounds);

        int reWins() const;
        void setReWins(const int reWins);

        int contraRounds() const;
        void setContraRounds(const int contraRounds);

        int contraWins() const;
        void setContraWins(const int contraWins);

        int placement() const;
        void setPlacement(const int placement);

        int points() const;
        void setPoints(const int points);

    private:
        QpWeakRelation<Game> m_game;
        QpWeakRelation<Player> m_player;

        int m_hochzeiten;
        int m_soli;
        int m_trumpfabgaben;

        int m_reRounds;
        int m_reWins;
        int m_contraRounds;
        int m_contraWins;

        int m_placement;
        int m_points;

};

#endif // DOKOOFFLINEGAME_H
