#ifndef GAME_H
#define GAME_H

#include "parseobject.h"

#include <QPersistence.h>
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

class Game: public ParseObject
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_ENUMS(State)
    Q_ENUMS(AdditionalMissingPlayer)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(Type type READ type WRITE setType)
    Q_PROPERTY(AdditionalMissingPlayer aditionalMissingPlayer READ aditionalMissingPlayer WRITE setAdditionalMissingPlayer)
    Q_PROPERTY(QDateTime creationTime READ creationTime WRITE setCreationTime)
    Q_PROPERTY(QString comment READ comment WRITE setComment)
    Q_PROPERTY(bool mitPflichtSolo READ mitPflichtSolo WRITE setMitPflichtSolo)
    Q_PROPERTY(QSharedPointer<Place> site READ site WRITE setSite)
    Q_PROPERTY(QList<QSharedPointer<Player> > players READ players WRITE setPlayers)
    Q_PROPERTY(QList<QSharedPointer<Round> > rounds READ rounds WRITE setRounds)

    Q_PROPERTY(QList<QSharedPointer<OLD_OfflineGameInformation> > offlineGameInformation READ offlineGameInformation WRITE setOfflineGameInformation)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:site",
                "reverserelation=games")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:players",
                "reverserelation=games")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:rounds",
                "reverserelation=game")
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

    enum AdditionalMissingPlayer {
        MissingOppositeOfCardMixer,
        MissingInTurn
    };

    explicit Game(QObject *parent = 0);
    ~Game();

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
    void finish();

    QPixmap statePixmap() const;

    bool mitPflichtSolo() const;
    void setMitPflichtSolo(bool arg);

    QString playerString();

    AdditionalMissingPlayer aditionalMissingPlayer() const;
    void setAdditionalMissingPlayer(AdditionalMissingPlayer arg);

    QTime length() const;
    QTime finishedRoundsLength() const;
    QTime averageRoundLength(double weight = 0.7) const;
    QTime predictedTimeToPlay(double weight = 0.7);
    QTime predictedEndTime(double weight = 0.7);
    QTime predictedGameLength(double weight = 0.7);
    QDateTime endDate() const;

    QSharedPointer<Place> site() const;

    QList<QSharedPointer<Player> > players() const;
    QSharedPointer<Player> currentCardMixer();
    QList<QSharedPointer<Player> > currentPlayingPlayers();

    QList<QSharedPointer<Round> > rounds() const;
    QSharedPointer<Round> currentRound() const;

    QList<QSharedPointer<Player> > playersSortedByPlacement() const;
    int placement(QSharedPointer<Player> player);
    double averagePlacement(QSharedPointer<Player> player) const;
    int leadingRoundCount(QSharedPointer<Player> player) const;
    int totalPoints() const;
    int totalPoints(QSharedPointer<Player> player) const;
    int roundsTogether(QSharedPointer<Player> player1, QSharedPointer<Player> player2);
    int winsTogether(QSharedPointer<Player> player1, QSharedPointer<Player> player2);
    int pointsToLeader(QSharedPointer<Player> player);

    int drinkCount(QSharedPointer<Drink> drink) const;
    int drinkCount(QSharedPointer<Player> player) const;
    QMap<QSharedPointer<Drink>, int> drinkCounts(QSharedPointer<Player> player) const;

    QList<QSharedPointer<LiveDrink> > liveDrinks() const;
    QList<QSharedPointer<LiveDrink> > liveDrinks(QSharedPointer<Player> player) const;

    void startNextRound();
    void save();

    int totalRoundCount();
    int finishedRoundCount();
    int roundsToPlay();
    double completedPercentage();
    bool isComplete();

    bool hasPflichtSolo(QSharedPointer<Player> player) const;

    int normalRoundCount(int roundCount = std::numeric_limits<int>::max());
    int hochzeitCount(int roundCount = std::numeric_limits<int>::max());
    int soloCount(int roundCount = std::numeric_limits<int>::max());
    int pflichtSoloCount(int roundCount = std::numeric_limits<int>::max());
    int trumpfabgabeCount(int roundCount = std::numeric_limits<int>::max());
    int schweinereiCount(int roundCount = std::numeric_limits<int>::max());
    int schmeissereiCount(int roundCount = std::numeric_limits<int>::max());

    int reWinsCount();
    int contraWinCount();

    int reGamePoints();
    int contraGamePoints();

    QList<QSharedPointer<Round> > rounds(QSharedPointer<Player> player);
    QList<QSharedPointer<Round> > reRounds(QSharedPointer<Player> player);
    QList<QSharedPointer<Round> > contraRounds(QSharedPointer<Player> player);

    QList<QSharedPointer<OLD_OfflineGameInformation> > offlineGameInformation() const;
    QList<QSharedPointer<OLD_DokoOfflineGameBuddys> > dokoOfflineGameBuddys() const;

signals:
    void newRoundStarted();
    void schmeissereiAdded();
    void stateChanged();
    void lengthChanged();
    void drinksChanged();

public slots:
    void setSite(QSharedPointer<Place> site);
    void addPlayer(QSharedPointer<Player> player);

private slots:
    void setRounds(const QList<QSharedPointer<Round> > &rounds);
    void addRound(QSharedPointer<Round> round);
    void removeRound(QSharedPointer<Round> round);

    void setPlayers(const QList<QSharedPointer<Player> > &players);
    void removePlayer(QSharedPointer<Player> player);

    // Stats-Sachen
    void calcInitialStats();
    void updateStats();

    void addToRePlayersStats(QSharedPointer<Round> round, QSharedPointer<Player> player);
    void addToContraPlayersStats(QSharedPointer<Round> round, QSharedPointer<Player> player);
    void addToGamesTogetherStats(QSharedPointer<Round> round, QSharedPointer<Player> player1, QSharedPointer<Player> player2);
private:
    void setCreationTime(const QDateTime &creationTime);
    void setOfflineGameInformation(const QList<QSharedPointer<OLD_OfflineGameInformation> > &games);
    void setDokoOfflineGameBuddys(const QList<QSharedPointer<OLD_DokoOfflineGameBuddys> > &games);

    void connectRoundSignals(QSharedPointer<Round> round);
    void connectAllRoundSignals();

    QString m_name;
    Type m_type;
    QDateTime m_creationTime;
    QString m_comment;
    bool m_mitPflichtSolo;
    AdditionalMissingPlayer m_additionalMissingPlayer;

    mutable QSharedPointer<Round> m_currentRoundCached;

    QpHasOne<Place> m_site;
    QpHasMany<Player> m_players;
    QpHasMany<Round> m_rounds;
    QpHasMany<OLD_OfflineGameInformation> m_offlineGameInformation;

    QTimer m_lengthTimer;


    // Stats-Sachen
    bool m_initialCalcedStats;

    int m_finishedRoundCount;
    int m_totalRoundCount;

    int m_reWinCount;
    int m_contraWinCount;

    int m_reGamePoints;
    int m_contraGamePoints;

    QHash<QSharedPointer<Player>,int> m_placements;

    QHash<QSharedPointer<Player>,QList<QSharedPointer<Round> > > m_playerReRounds;
    QHash<QSharedPointer<Player>,QList<QSharedPointer<Round> > > m_playerContraRounds;

    QHash<QString, QList<QSharedPointer<Round> > > m_playerRoundsTogether;
    QHash<QString, QList<QSharedPointer<Round> > > m_playerRoundWinsTogether;
};

bool sortGamesByDate(const QSharedPointer<Game> &g1, const QSharedPointer<Game> &g2);
bool sortGamesByDateLastFirst(const QSharedPointer<Game> &g1, const QSharedPointer<Game> &g2);


#endif // GAME_H
