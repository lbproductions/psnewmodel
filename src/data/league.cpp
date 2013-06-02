#include "league.h"

#include "game.h"
#include "player.h"

League::League(QObject *parent) :
    QObject(parent),
    m_playerRatio(0.75),
    m_players("players", this),
    m_games("games",this)
{
}

QDate League::startDate() const
{
    return m_startDate;
}

void League::setStartDate(const QDate &startDate)
{
    m_startDate = startDate;
}

QDate League::endDate() const
{
    return m_endDate;
}

void League::setEndDate(const QDate &endDate)
{
    m_endDate = endDate;
}

QList<QSharedPointer<Player> > League::players() const
{
    return m_players.resolveList();
}

QList<QSharedPointer<Game> > League::games() const
{
    return m_games.resolveList();
}

void League::setPlayers(const QList<QSharedPointer<Player> > &players)
{
    m_players.relate(players);
}

void League::setGames(const QList<QSharedPointer<Game> > &games)
{
    m_games.relate(games);
}

bool League::hasEnoughPlayers(QSharedPointer<Game> game) const
{
    QList<QSharedPointer<Player> > ps = players();
    QList<QSharedPointer<Player> > gps = game->players();

    if(gps.size() == 0)
        return false;

    int leaguePlayerCount = 0;
    foreach(QSharedPointer<Player> p, gps) {
        bool isLeaguePlayer = false;
        foreach(QSharedPointer<Player> lp, ps) {
            if(p == lp) {
                isLeaguePlayer = true;
                break;
            }
        }

        if(isLeaguePlayer)
            ++leaguePlayerCount;
    }

    double ratio = (double) leaguePlayerCount / (double) gps.size();
    return ratio >= m_playerRatio;
}

QList<QSharedPointer<Game> > League::calculatePossibleGames() const
{
    QList<QSharedPointer<Game> > possibleGames;
    foreach(QSharedPointer<Player> p, players()) {
        possibleGames.append(p->games());
    }

    if(possibleGames.isEmpty())
        return QList<QSharedPointer<Game> >();

    qSort(possibleGames.begin(), possibleGames.end(), &sortGamesByDate);

    QSharedPointer<Game> g = possibleGames.first();
    while(!possibleGames.isEmpty() && g->creationTime().date() < startDate())
        g = possibleGames.takeFirst();

    if(possibleGames.isEmpty())
        return QList<QSharedPointer<Game> >();

    g = possibleGames.last();
    while(!possibleGames.isEmpty() && g->creationTime().date() > endDate())
        g = possibleGames.takeLast();

    for(int i = 0; i < possibleGames.size(); ++i) {
        if(!hasEnoughPlayers(possibleGames.at(i)))
            possibleGames.removeAt(i);
    }

    return possibleGames;
}
