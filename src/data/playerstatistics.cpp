#include "playerstatistics.h"

#include "player.h"
#include "game.h"
#include "round.h"
#include "schmeisserei.h"

#include <limits>

PlayerStatistics::PlayerStatistics(QObject *parent) :
    QObject(parent)
{
}

bool PlayerStatistics::isAllGames() const
{
    return m_games.isEmpty();
}

QSharedPointer<Player> PlayerStatistics::player() const
{
    return Qp::sharedFrom(m_player);
}

void PlayerStatistics::setPlayer(Player *player)
{
    m_player = player;
}

QList<QSharedPointer<Game> > PlayerStatistics::games() const
{
    if(isAllGames()) {
        return player()->games();
    }

    return m_games;
}

void PlayerStatistics::setGames(const QList<QSharedPointer<Game> > &games)
{
    m_games = games;
}

QList<QSharedPointer<Game> > PlayerStatistics::gamesByDate() const
{
    QList<QSharedPointer<Game> > gs = games();
    qSort(gs.begin(), gs.end(), &sortGamesByDate);
    return gs;
}

QList<QSharedPointer<Game> > PlayerStatistics::wins() const
{
    QList<QSharedPointer<Game> > result;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        QList<QSharedPointer<Player> > players = game->playersSortedByPlacement();
        if(players.isEmpty())
            continue;

        int count = players.size();
        int previousPoints = std::numeric_limits<int>::min();
        for(int i = 0; i < count; ++i) {
            QSharedPointer<Player> player = players.at(i);
            if(player == sharedPlayer) {
                result.append(game);
                break;
            }

            int currentPoints = game->totalPoints(player);
            if(currentPoints < previousPoints) {
                break;
            }
            previousPoints = currentPoints;
        }
    }
    return result;
}

QList<QSharedPointer<Game> > PlayerStatistics::losses() const
{
    QList<QSharedPointer<Game> > result;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        QList<QSharedPointer<Player> > players = game->playersSortedByPlacement();
        if(players.isEmpty())
            continue;

        int count = players.size();
        int previousPoints = std::numeric_limits<int>::max();
        for(int i = count - 1; i >= 0; --i) {
            QSharedPointer<Player> player = players.at(i);
            if(player == sharedPlayer) {
                result.append(game);
                break;
            }

            int currentPoints = game->totalPoints(player);
            if(currentPoints > previousPoints) {
                break;
            }
            previousPoints = currentPoints;
        }
    }
    return result;
}

QSharedPointer<Game> PlayerStatistics::lastGame() const
{
    QList<QSharedPointer<Game> > gs = gamesByDate();
    if(gs.isEmpty())
        return QSharedPointer<Game>();

    return gs.first();
}

QSharedPointer<Game> PlayerStatistics::lastWin() const
{
    QList<QSharedPointer<Game> > gs = gamesByDate();
    if(gs.isEmpty())
        return QSharedPointer<Game>();

    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, gs) {
        if(game->placement(sharedPlayer) == 1)
            return game;
    }

    return QSharedPointer<Game>();
}

QList<QSharedPointer<Round> > PlayerStatistics::rounds() const
{
    if(isAllGames())
        return player()->rounds();

    QList<QSharedPointer<Round> > result;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(round->playingPlayers().contains(sharedPlayer))
                result.append(round);
        }
    }

    return result;
}

QList<QSharedPointer<Round> > PlayerStatistics::winRounds() const
{
    QList<QSharedPointer<Round> > result = reWins();
    result.append(contraWins());
    return result;
}

QList<QSharedPointer<Round> > PlayerStatistics::reRounds() const
{
    if(isAllGames())
        return player()->reRounds();

    QList<QSharedPointer<Round> > result;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(round->re1Player() == sharedPlayer
                    || (!round->isSolo() || round->re2Player() == sharedPlayer)) {
                result.append(round);
            }
        }
    }

    return result;
}

QList<QSharedPointer<Round> > PlayerStatistics::reWins() const
{
    QList<QSharedPointer<Round> > result;
    foreach(QSharedPointer<Round> round, reRounds()) {
        if(round->winnerParty() == Round::Re) {
            result.append(round);
        }
    }
    return result;
}

QList<QSharedPointer<Round> > PlayerStatistics::contraRounds() const
{
    if(isAllGames())
        return player()->contraRounds();

    QList<QSharedPointer<Round> > result;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(round->contra1Player() == sharedPlayer
                    || round->contra2Player() == sharedPlayer
                    || (round->isSolo() && round->contra3Player() == sharedPlayer)) {
                result.append(round);
            }
        }
    }

    return result;
}

QList<QSharedPointer<Round> > PlayerStatistics::contraWins() const
{
    QList<QSharedPointer<Round> > result;
    foreach(QSharedPointer<Round> round, contraRounds()) {
        if(round->winnerParty() == Round::Contra) {
            result.append(round);
        }
    }
    return result;
}

QList<QSharedPointer<Round> > PlayerStatistics::hochzeitRounds() const
{
    if(isAllGames())
        return player()->hochzeitRounds();

    QList<QSharedPointer<Round> > result;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(round->hochzeitPlayer() == sharedPlayer) {
                result.append(round);
            }
        }
    }

    return result;
}

QList<QSharedPointer<Round> > PlayerStatistics::trumpfabgabeRounds() const
{
    if(isAllGames())
        return player()->trumpfabgabeRounds();

    QList<QSharedPointer<Round> > result;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(round->trumpfabgabePlayer() == sharedPlayer) {
                result.append(round);
            }
        }
    }

    return result;
}

QList<QSharedPointer<Round> > PlayerStatistics::soloRounds() const
{
    if(isAllGames())
        return player()->soloRounds();

    QList<QSharedPointer<Round> > result;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(round->soloPlayer() == sharedPlayer) {
                result.append(round);
            }
        }
    }

    return result;
}

QList<QSharedPointer<Schmeisserei> > PlayerStatistics::schmeissereien() const
{
    if(isAllGames())
        return player()->schmeissereien();

    QList<QSharedPointer<Schmeisserei> > result;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            foreach(QSharedPointer<Schmeisserei> s, round->schmeissereien()) {
                if(s->player() == sharedPlayer) {
                    result.append(s);
                }
            }
        }
    }

    return result;
}

QList<QSharedPointer<Round> > PlayerStatistics::schweinereiRounds() const
{
    if(isAllGames())
        return player()->schweinereiRounds();

    QList<QSharedPointer<Round> > result;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(round->schweinereiPlayer() == sharedPlayer) {
                result.append(round);
            }
        }
    }

    return result;
}

double PlayerStatistics::roundWinsPercentage() const
{
    return percentage(winRounds().size(), rounds().size());
}

double PlayerStatistics::rePercentage() const
{
    return percentage(reRounds().size(), rounds().size());
}

double PlayerStatistics::reWinsPercentage() const
{
    return percentage(reWins().size(), reRounds().size());
}

double PlayerStatistics::contraPercentage() const
{
    return percentage(contraRounds().size(), rounds().size());
}

double PlayerStatistics::contraWinsPercentage() const
{
    return percentage(contraWins().size(), contraRounds().size());
}

double PlayerStatistics::averagePointsPerRound() const
{
    return percentage(winRounds().size(), rounds().size());
}

double PlayerStatistics::averagePlacement() const
{
    QList<QSharedPointer<Game> > gs = games();
    int gameCount = gs.size();
    if(gameCount == 0)
        return 0.0;

    double result = 0.0;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, gs) {
        result += game->placement(sharedPlayer);
    }
    return (double) result / (double) gameCount;
}

int PlayerStatistics::gamePoints() const
{
    int points = 0;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        points += game->totalPoints(sharedPlayer);
    }
    return points;
}

int PlayerStatistics::points() const
{
    int points = 0;
    QSharedPointer<Player> sharedPlayer = player();
    foreach(QSharedPointer<Game> game, games()) {
        int playerCount = game->players().size();
        int zaehler = playerCount - game->placement(sharedPlayer);
        int nenner = playerCount - 1;
        points += 100 * zaehler / nenner;
    }
    return points;
}

double PlayerStatistics::average() const
{
    return percentage(points(), games().size());
}

double PlayerStatistics::percentage(int value1, int value2)
{
    if(value2 == 0)
        return -1.0;

    return (double) value1 / (double) value2;
}
