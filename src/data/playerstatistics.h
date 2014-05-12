#ifndef PLAYERSTATISTICS_H
#define PLAYERSTATISTICS_H

#include <QObject>

#include <QSharedPointer>

class Player;
class Game;
class Round;
class Schmeisserei;

class PlayerStatistics : public QObject
{
    Q_OBJECT
public:
    explicit PlayerStatistics(QObject *parent = 0);

    bool isAllGames() const;

    QSharedPointer<Player> player() const;
    void setPlayer(Player *player);

    QList<QSharedPointer<Game> > games() const;
    void setGames(const QList<QSharedPointer<Game> > &games);

    QList<QSharedPointer<Game> > gamesByDate() const;

    QList<QSharedPointer<Game> > wins() const;
    QList<QSharedPointer<Game> > losses() const;

    QSharedPointer<Game> lastGame() const;
    QSharedPointer<Game> lastWin() const;

    QList<QSharedPointer<Round> > rounds() const;
    QList<QSharedPointer<Round> > winRounds() const;
    QList<QSharedPointer<Round> > reRounds() const;
    QList<QSharedPointer<Round> > reWins() const;
    QList<QSharedPointer<Round> > contraRounds() const;
    QList<QSharedPointer<Round> > contraWins() const;
    QList<QSharedPointer<Round> > hochzeitRounds() const;
    QList<QSharedPointer<Round> > trumpfabgabeRounds() const;
    QList<QSharedPointer<Round> > soloRounds() const;
    QList<QSharedPointer<Schmeisserei> > schmeissereien() const;
    QList<QSharedPointer<Round> > schweinereiRounds() const;

    double roundWinsPercentage() const;
    double rePercentage() const;
    double reWinsPercentage() const;
    double contraPercentage() const;
    double contraWinsPercentage() const;

    double averagePointsPerRound() const;
    double averageRePointsPerRound() const;
    double averageContraPointsPerRound() const;
    double averagePlacement() const;

    int gamePoints() const;

    int points() const;
    double average() const;

    QTime averageRoundTime();
    QTime averageSoloTime();

    int lostServe();
    QList<QSharedPointer<Round> > serveRounds() const;

private:
    Player *m_player;
    QList<QSharedPointer<Game> > m_games;

    static double percentage(int value1, int value2);
};

#endif // PLAYERSTATISTICS_H
