#ifndef GAME_H
#define GAME_H

#include <QObject>

#include <QPersistenceRelations.h>
#include <QTime>
#include <QDateTime>
#include <QTimer>



class Round;
class Place;
class Player;
class LiveDrink;

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

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:site",
                "reverserelation=games")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:players",
                "reverserelation=games")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:rounds",
                "reverserelation=game")
    Q_CLASSINFO("QPERSISTENCE_SQLFILTER",
                "typeString = 'Doppelkopf' AND live = 'True'")

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

    bool mitPflichtSolo() const;
    void setMitPflichtSolo(bool arg);

    QTime length() const;

    QSharedPointer<Place> site() const;

    QList<QSharedPointer<Player> > players() const;
    QSharedPointer<Player> currentCardMixer() const;
    QList<QSharedPointer<Player> > currentPlayingPlayers() const;

    QList<QSharedPointer<Round> > rounds() const;
    QSharedPointer<Round> currentRound() const;

    QList<QSharedPointer<Player> > playersSortedByPlacement() const;
    int placement(QSharedPointer<Player> player) const;
    int placementAfterRounds(QSharedPointer<Player> player, int roundNumber) const;
    double averagePlacement(QSharedPointer<Player> player) const;
    int leadingRoundCount(QSharedPointer<Player> player) const;
    int totalPoints() const;
    int totalPoints(QSharedPointer<Player> player) const;

    QList<QSharedPointer<LiveDrink> > drinks() const;
    QList<QSharedPointer<LiveDrink> > drinks(QSharedPointer<Player> player) const;

    double completedPercentage() const;
    bool isComplete() const;

    bool hasPflichtSolo(QSharedPointer<Player> player) const;

    int hochzeitCountAfterRounds(int roundCount);
    int soloCountAfterRounds(int roundCount);
    int pflichtSoloCountAfterRounds(int roundCount);
    int trumpfabgabeCountAfterRounds(int roundCount);
    int schweinereiCountAfterRounds(int roundCount);
    int schmeissereiCountAfterRounds(int roundCount);

private:
    void setCreationTime(const QDateTime &creationTime);
    void setSite(QSharedPointer<Place> site);
    void setPlayers(const QList<QSharedPointer<Player> > &players);
    void setRounds(const QList<QSharedPointer<Round> > &rounds);

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

    QTimer m_lengthTimer;
};



#endif // GAME_H
