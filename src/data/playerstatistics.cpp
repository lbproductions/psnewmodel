#include "playerstatistics.h"

#include "player.h"
#include "game.h"
#include "round.h"
#include "schmeisserei.h"

#include "old_offlineGameInformation.h"

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

        int pointsOfWinner = game->totalPoints(players.first());
        int count = players.size();
        for(int i = 0; i < count; i++) {
            QSharedPointer<Player> player = players.at(i);

            if(game->totalPoints(player) != pointsOfWinner) {
                break;
            }

            if(player == sharedPlayer) {
                result.append(game);
                break;
            }
        }
    }
    // TODO: Remove or change these loop into current data structure
    if(isAllGames()) {
        foreach(QSharedPointer<OLD_OfflineGameInformation> info, player()->offlineGameInformation()) {
            if(info->placement() == 1) {
                if(!result.contains(info->game())){
                    result.append(info->game());
                }
            }
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

        int pointsOfLast = game->totalPoints(players.last());
        int count = players.size();
        for(int i = count - 1; i >= 0; --i) {
            QSharedPointer<Player> player = players.at(i);

            if(game->totalPoints(player) != pointsOfLast) {
                break;
            }

            if(player == sharedPlayer) {
                result.append(game);
                break;
            }
        }
    }
    // TODO: Remove or change these loop into current data structure
    if(isAllGames()) {
        foreach(QSharedPointer<OLD_OfflineGameInformation> info, player()->offlineGameInformation()) {
            if(info->placement() == info->game()->offlineGameInformation().size()) {
                result.append(info->game());
            }
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

    QList<QSharedPointer<Round> > result1 = reRounds();
    result1.append(contraRounds());

    return result1;
}

QList<QSharedPointer<Round> > PlayerStatistics::winRounds() const
{
    /*
    QList<QSharedPointer<Round> > result;
    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(round->points(player()) > 0) {
                result.append(round);
            }
        }
    }
    return result;
    */
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
            if(round->re1Player() == sharedPlayer || (!round->isSolo() && round->re2Player() == sharedPlayer)) {
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
    return percentage(winRounds().size() * 100, rounds().size());
}

double PlayerStatistics::rePercentage() const
{
    return percentage(reRounds().size() * 100, rounds().size());
}

double PlayerStatistics::reWinsPercentage() const
{
    return percentage(reWins().size() * 100, reRounds().size());
}

double PlayerStatistics::contraPercentage() const
{
    return percentage(contraRounds().size() * 100, rounds().size());
}

double PlayerStatistics::contraWinsPercentage() const
{
    return percentage(contraWins().size() * 100, contraRounds().size());
}

double PlayerStatistics::averagePointsPerRound() const
{
    int points = 0;
    foreach(QSharedPointer<Round> round, winRounds()) {
        points += round->points(player());
    }
    return percentage(points, winRounds().size());
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
    // TODO: Remove or change these loop into current data structure
    if(isAllGames()) {
        foreach(QSharedPointer<OLD_OfflineGameInformation> info, player()->offlineGameInformation()) {
            int playerCount = info->game()->offlineGameInformation().size();
            int zaehler = playerCount - info->placement();
            int nenner = playerCount - 1;
            points += 100 * zaehler / nenner;
        }
    }

    return points;
}

double PlayerStatistics::average() const
{
    // TODO: Merge or change the offlineGameInformations
    if(isAllGames()) {
        return percentage(points(), games().size()+player()->offlineGameInformation().size());
    }
    else{
        return percentage(points(), games().size());
    }
}

double PlayerStatistics::percentage(int value1, int value2)
{
    if(value2 == 0)
        return 0;

    double number = (double) value1 / (double) value2;

    return number;
}
