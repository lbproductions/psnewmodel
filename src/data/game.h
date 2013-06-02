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

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:site",
                "reverserelation=games")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:players",
                "reverserelation=games")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:rounds",
                "reverserelation=game")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:leagues",
                "reverserelation=games")

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

    QString name() const;
    void setName(const QString &name);

    Type type() const;
    void setType(const Type &type);

    QDateTime creationTime() const;

    QString comment() const;
    void setComment(const QString &comment);

    State state() const;
    void setState(State state);

    QPixmap statePixmap() const;

    bool mitPflichtSolo() const;
    void setMitPflichtSolo(bool arg);

    QTime length() const;

    QSharedPointer<Place> site() const;

    QList<QSharedPointer<League> > leagues() const;

    QList<QSharedPointer<Player> > players() const;
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

    QList<QSharedPointer<LiveDrink> > drinks() const;
    QList<QSharedPointer<LiveDrink> > drinks(QSharedPointer<Player> player) const;

    double completedPercentage() const;
    bool isComplete() const;

    bool hasPflichtSolo(QSharedPointer<Player> player) const;

    int hochzeitCount(int roundCount = std::numeric_limits<int>::max());
    int soloCount(int roundCount = std::numeric_limits<int>::max());
    int pflichtSoloCount(int roundCount = std::numeric_limits<int>::max());
    int trumpfabgabeCount(int roundCount = std::numeric_limits<int>::max());
    int schweinereiCount(int roundCount = std::numeric_limits<int>::max());
    int schmeissereiCount(int roundCount = std::numeric_limits<int>::max());

private:
    void setCreationTime(const QDateTime &creationTime);
    void setSite(QSharedPointer<Place> site);
    void setPlayers(const QList<QSharedPointer<Player> > &players);
    void setRounds(const QList<QSharedPointer<Round> > &rounds);
    void setLeagues(const QList<QSharedPointer<League> > &arg);

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

    QTimer m_lengthTimer;
};

bool sortGamesByDate(const QSharedPointer<Game> &g1, const QSharedPointer<Game> &g2);


#endif // GAME_H
