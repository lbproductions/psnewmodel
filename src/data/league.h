#ifndef LEAGUE_H
#define LEAGUE_H

#include <QObject>

#include <QPersistenceRelations.h>
#include <QDate>

class Player;
class Game;
class PlayerStatistics;

class League : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate)
    Q_PROPERTY(QDate endDate READ endDate WRITE setEndDate)
    Q_PROPERTY(QList<QSharedPointer<Game> > games READ games WRITE setGames)
    Q_PROPERTY(QList<QSharedPointer<Player> > players READ players WRITE setPlayers)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:games",
                "reverserelation=leagues")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:players",
                "reverserelation=leagues")


public:
    explicit League(QObject *parent = 0);

    QDate startDate() const;
    void setStartDate(const QDate &startDate);

    QDate endDate() const;
    void setEndDate(const QDate &endDate);

    QList<QSharedPointer<Player> > players() const;
    QList<QSharedPointer<Game> > games() const;

    QList<QSharedPointer<Game> > calculatePossibleGames() const;

private:
    void setPlayers(const  QList<QSharedPointer<Player> > &players);
    void setGames(const  QList<QSharedPointer<Game> > &games);

    QDate m_startDate;
    QDate m_endDate;

    double m_playerRatio;

    QpWeakRelation<Player> m_players;
    QpWeakRelation<Game> m_games;

    QHash<QSharedPointer<Player>, QSharedPointer<PlayerStatistics> > m_playerStatistics;

    bool hasEnoughPlayers(QSharedPointer<Game> game) const;
};

#endif // LEAGUE_H
