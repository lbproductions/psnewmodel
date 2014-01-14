#ifndef GAME_H
#define GAME_H

#include <QObject>

#include <QPersistenceRelations.h>
#include <QTime>
#include <QDateTime>
#include <QTimer>
#include <limits>

class Round;
class Place;
class Player;
class LiveDrink;
class League;
class Drink;
class OLD_OfflineGameInformation;
class OLD_DokoOfflineGameBuddys;

class Game: public QObject
{
        Q_OBJECT
        Q_ENUMS(Type)
        Q_ENUMS(State)
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(int type READ _type WRITE _setType)
        Q_PROPERTY(QDateTime creationTime READ creationTime WRITE setCreationTime)
        Q_PROPERTY(QString comment READ comment WRITE setComment)
        Q_PROPERTY(bool mitPflichtSolo READ mitPflichtSolo WRITE setMitPflichtSolo)
        Q_PROPERTY(QSharedPointer<Place> site READ site WRITE setSite)
        Q_PROPERTY(QList<QSharedPointer<Player> > players READ players WRITE setPlayers)
        Q_PROPERTY(QList<QSharedPointer<Round> > rounds READ rounds WRITE setRounds)
        Q_PROPERTY(QList<QSharedPointer<League> > leagues READ leagues WRITE setLeagues)

        Q_PROPERTY(QList<QSharedPointer<OLD_OfflineGameInformation> > offlineGameInformation READ offlineGameInformation WRITE setOfflineGameInformation)
        Q_PROPERTY(QList<QSharedPointer<OLD_DokoOfflineGameBuddys> > dokoOfflineGameBuddys READ dokoOfflineGameBuddys WRITE setDokoOfflineGameBuddys)

        Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:site",
                    "reverserelation=games")
        Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:players",
                    "reverserelation=games")
        Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:rounds",
                    "reverserelation=game")
        Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:leagues",
                    "reverserelation=games")
        Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:offlineGameInformation",
                    "reverserelation=game")
        Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:dokoOfflineGameBuddys",
                    "reverserelation=game")

    public:
        enum Type {
            UnkownType,
            Doppelkopf,
            Skat,
            Hearts,
            Prognose
        };

        enum State {
            UnkownState,
            Running,
            Paused,
            Finished
        };

        explicit Game(QObject *parent = 0);
        ~Game();

        void save();

        QString name() const;
        void setName(const QString &name);

        Type type() const;
        void setType(const Type &type);

        QDateTime creationTime() const;

        QString comment() const;
        void setComment(const QString &comment);

        State state() const;
        void setState(State state);
        void togglePlayPause();
        void pause();

        QPixmap statePixmap() const;

        bool mitPflichtSolo() const;
        void setMitPflichtSolo(bool arg);

        QTime length() const;
        QTime finishedRoundsLength() const;
        QTime averageRoundLength(double weight = 0.7) const;
        QTime predictedTimeToPlay(double weight = 0.7) const;
        QTime predictedEndTime(double weight = 0.7) const;

        QSharedPointer<Place> site() const;
        void setSite(QSharedPointer<Place> site);

        QList<QSharedPointer<League> > leagues() const;

        QList<QSharedPointer<Player> > players() const;
        void addPlayer(QSharedPointer<Player> player);
        QSharedPointer<Player> currentCardMixer() const;
        QList<QSharedPointer<Player> > currentPlayingPlayers() const;

        QList<QSharedPointer<Round> > rounds() const;
        QSharedPointer<Round> currentRound() const;

        QList<QSharedPointer<Player> > playersSortedByPlacement() const;
        int placement(QSharedPointer<Player> player, int roundNumber = std::numeric_limits<int>::max()) const;
        double averagePlacement(QSharedPointer<Player> player) const;
        int leadingRoundCount(QSharedPointer<Player> player) const;
        int totalPoints() const;
        int totalPoints(QSharedPointer<Player> player) const;
        int roundsTogether(QSharedPointer<Player> playerOne, QSharedPointer<Player> playerTwo);
        int winsTogether(QSharedPointer<Player> playerOne, QSharedPointer<Player> playerTwo);
        int pointsToLeader(QSharedPointer<Player> player);

        void addDrink(QSharedPointer<LiveDrink> drink);
        QMap<QSharedPointer<Drink>, int> drinkCounts(QSharedPointer<Player> player) const;
        QList<QSharedPointer<LiveDrink> > liveDrinks() const;
        QList<QSharedPointer<LiveDrink> > liveDrinks(QSharedPointer<Player> player) const;

        void startNextRound();

        int totalRoundCount() const;
        int finishedRoundCount() const;
        double completedPercentage() const;
        bool isComplete() const;
        int roundsToPlay() const;

        bool hasPflichtSolo(QSharedPointer<Player> player) const;

        int hochzeitCount(int roundCount = std::numeric_limits<int>::max());
        int soloCount(int roundCount = std::numeric_limits<int>::max());
        int pflichtSoloCount(int roundCount = std::numeric_limits<int>::max());
        int trumpfabgabeCount(int roundCount = std::numeric_limits<int>::max());
        int schweinereiCount(int roundCount = std::numeric_limits<int>::max());
        int schmeissereiCount(int roundCount = std::numeric_limits<int>::max());

        int reWinsCount();
        int contraWinCount();

        QList<QSharedPointer<OLD_OfflineGameInformation> > offlineGameInformation() const;
        QList<QSharedPointer<OLD_DokoOfflineGameBuddys> > dokoOfflineGameBuddys() const;
    signals:
        void newRoundStarted();
        void schmeissereiAdded();
        void liveDrinkAdded();
        void stateChanged();
        void lengthChanged();

    private:
        void setCreationTime(const QDateTime &creationTime);
        void setPlayers(const QList<QSharedPointer<Player> > &players);
        void setRounds(const QList<QSharedPointer<Round> > &rounds);
        void addRound(QSharedPointer<Round> round);
        void setLeagues(const QList<QSharedPointer<League> > &arg);
        void setOfflineGameInformation(const QList<QSharedPointer<OLD_OfflineGameInformation> > &games);
        void setDokoOfflineGameBuddys(const QList<QSharedPointer<OLD_DokoOfflineGameBuddys> > &games);

        int _type() const;
        void _setType(int type);

        QString m_name;
        Type m_type;
        QDateTime m_creationTime;
        QString m_comment;
        bool m_mitPflichtSolo;

        QpWeakRelation<Place> m_site;
        QpStrongRelation<Player> m_players;
        QpStrongRelation<Round> m_rounds;
        QpWeakRelation<League> m_leagues;

        QpStrongRelation<OLD_OfflineGameInformation> m_offlineGameInformation;
        QpStrongRelation<OLD_DokoOfflineGameBuddys> m_dokoOfflineGameBuddys;

        QTimer m_lengthTimer;
};

bool sortGamesByDate(const QSharedPointer<Game> &g1, const QSharedPointer<Game> &g2);


#endif // GAME_H
