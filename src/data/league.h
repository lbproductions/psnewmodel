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

        QList<QSharedPointer<Game> > calculatedGames();
        QList<QSharedPointer<Game> > calculatePossibleGames() const;
        QList<QSharedPointer<Game> > filteredGames();


        QSharedPointer<PlayerStatistics> playerStats(QSharedPointer<Player> player);

        double playerRatio() const;
        void setPlayerRatio(const double ratio);

        QString name() const;
        void setName(const QString name);

        void recalculate();

        double finishedGamesBorder() const;
        void setFinishedGamesBorder(double border);

        QSharedPointer<Matchday> currentMatchday();
        QList<QSharedPointer<Matchday> > matchdays();

    private:
        QList<QPair<QSharedPointer<Player>, double> > sortPlayersAfterAverage();

        void calculateMatchdays();

        QDate m_startDate;
        QDate m_endDate;

        QString m_name;

        double m_playerRatio;
        double m_finishedGamesPercentage;

        QpWeakRelation<Player> m_players;

        QList<QSharedPointer<Game> > m_calculatedGames;
        QList<QSharedPointer<Game> > m_filterGames;
        QList<QSharedPointer<Matchday> > m_matchdays;

        QHash<QSharedPointer<Player>, QSharedPointer<PlayerStatistics> > m_playerStatistics;

        bool hasEnoughPlayers(QSharedPointer<Game> game) const;
};

#endif // LEAGUE_H
