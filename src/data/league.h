#ifndef LEAGUE_H
#define LEAGUE_H

#include <QObject>

#include <QPersistenceRelations.h>
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

        QList<QSharedPointer<Player> > players() const;
        void setPlayers(const  QList<QSharedPointer<Player> > &players);

        QList<QSharedPointer<Game> > games() const;
        QList<QSharedPointer<Game> > calculatedGames();
        QList<QSharedPointer<Game> > calculatePossibleGames() const;

        QSharedPointer<PlayerStatistics> playerStats(QSharedPointer<Player> player);

        double playerRatio() const;
        void setPlayerRatio(const double ratio);

        QString name() const;
        void setName(const QString name);

        void recalculate();

        double finishedGamesBorder() const;
        void setFinishedGamesBorder(double border);

        static int currentMatchDayNumber;
        QSharedPointer<Matchday> currentMatchday();
        QList<QSharedPointer<Matchday> > matchdays();

    private:
        void setGames(const  QList<QSharedPointer<Game> > &games);

        QList<QPair<QSharedPointer<Player>, double> > sortPlayersAfterAverage();

        void calculateMatchdays();

        QDate m_startDate;
        QDate m_endDate;

        QString m_name;

        double m_playerRatio;
        double m_finishedGamesPercentage;

        QpWeakRelation<Player> m_players;
        QpWeakRelation<Game> m_games;

        QList<QSharedPointer<Game> > m_calculatedGames;
        QList<QSharedPointer<Matchday> > m_matchdays;

        QHash<QSharedPointer<Player>, QSharedPointer<PlayerStatistics> > m_playerStatistics;

        bool hasEnoughPlayers(QSharedPointer<Game> game) const;
};

#endif // LEAGUE_H
