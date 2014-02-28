#ifndef LEAGUE_H
#define LEAGUE_H

#include <QObject>

#include <QPersistence.h>
#include <QDate>

class Player;
class Game;
class PlayerStatistics;
class Matchday;

class League : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate)
    Q_PROPERTY(QDate endDate READ endDate WRITE setEndDate)
    Q_PROPERTY(double playerRatio READ playerRatio WRITE setPlayerRatio)
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

    double playerRatio() const;
    void setPlayerRatio(const double ratio);

    double finishedGamesBorder() const;
    void setFinishedGamesBorder(double border);

    QString name() const;
    void setName(const QString name);

    QList<QSharedPointer<Player> > players() const;
    QList<QSharedPointer<Game> > games() const;
    QList<QSharedPointer<Game> > calculatedGames();
    QList<QSharedPointer<Game> > calculatePossibleGames() const;
    QSharedPointer<PlayerStatistics> playerStats(QSharedPointer<Player> player);

    QSharedPointer<Matchday> currentMatchday();
    QList<QSharedPointer<Matchday> > matchdays();

    void recalculate();

    static int currentMatchDayNumber;

public slots:
    void setPlayers(const  QList<QSharedPointer<Player> > &players);
    void addPlayer(QSharedPointer<Player> player);
    void removePlayer(QSharedPointer<Player> player);

    void setGames(const  QList<QSharedPointer<Game> > &games);
    void addGame(QSharedPointer<Game> game);
    void removeGame(QSharedPointer<Game> game);

private:
    QList<QPair<QSharedPointer<Player>, double> > sortPlayersByAverage();
    void calculateMatchdays();
    bool hasEnoughPlayers(QSharedPointer<Game> game) const;

    QDate m_startDate;
    QDate m_endDate;
    QString m_name;
    double m_playerRatio;
    double m_finishedGamesPercentage;

    QpHasMany<Player> m_players;
    QpHasMany<Game> m_games;

    QList<QSharedPointer<Game> > m_calculatedGames;
    QList<QSharedPointer<Matchday> > m_matchdays;
    QHash<QSharedPointer<Player>, QSharedPointer<PlayerStatistics> > m_playerStatistics;
};

#endif // LEAGUE_H
