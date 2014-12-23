#ifndef PLAYERSTATISTICS_H
#define PLAYERSTATISTICS_H

#include <QObject>

#include <QSharedPointer>

class Player;
class Game;
class Round;
class Schmeisserei;

typedef QPair<QSharedPointer<Player>, double> PlayerDoublePair;
typedef QList<QSharedPointer<Round> > RoundSeries;

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
    QList<QSharedPointer<Round> > soloRounds();
    QList<QSharedPointer<Round> > soloWinRounds();
    QList<QSharedPointer<Round> > soloPflichtRounds();
    QList<QSharedPointer<Round> > soloPflichtWinRounds();
    QList<QSharedPointer<Round> > soloRounds(int type);
    QList<QSharedPointer<Round> > soloWinRounds(int type);
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

    QList<PlayerDoublePair> serverAveragePoints();

    QList<RoundSeries> series();
    QList<RoundSeries> winSeries();
    QList<RoundSeries> loseSeries();
    RoundSeries lastSeries();

 private:
    void initStats();
    void calcSeriesStats();
    void updateStats();

    bool m_statsInitialized;

    RoundSeries m_lastSeries;
    QList<RoundSeries> m_winSeries;
    QList<RoundSeries> m_loseSeries;

    QHash<int, QList<QSharedPointer<Round>>> m_soloTypeCounts;
    QList<QSharedPointer<Round>> m_soloRounds;
    QList<QSharedPointer<Round>> m_soloWinRounds;
    QList<QSharedPointer<Round>> m_soloPfichtRounds;
    QList<QSharedPointer<Round>> m_soloPflichtWinRounds;
    QHash<int, QList<QSharedPointer<Round>>> m_soloTypeWins;

    Player *m_player;
    QList<QSharedPointer<Game> > m_games;

    static double percentage(int value1, int value2);
};

bool sortPlayerDoublePair(PlayerDoublePair one, PlayerDoublePair two);
bool sortRoundsByDate(QSharedPointer<Round> one, QSharedPointer<Round> two);
bool sortRoundSeriesBySize(RoundSeries one, RoundSeries two);
bool sortRoundSeriesByDate(RoundSeries one, RoundSeries two);

#endif // PLAYERSTATISTICS_H
