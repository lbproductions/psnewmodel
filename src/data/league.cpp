#include "league.h"

#include "game.h"
#include "player.h"
#include "playerstatistics.h"
#include "matchday.h"


int League::currentMatchDayNumber = 1;

League::League(QObject *parent) :
    QObject(parent),
    m_playerRatio(0.75),
    m_finishedGamesPercentage(80),
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

QList<QPair<QSharedPointer<Player>, double> > League::sortPlayersAfterAverage()
{
    // too lazy to implement access of league-stats in sort-funtion out of this class
    QList<QPair<QSharedPointer<Player>,double> > result;

    QList<QSharedPointer<Player> > playersList = players();
    while(!playersList.isEmpty()) {
        double average = -1;
        int index = -1;
        foreach(QSharedPointer<Player> player, playersList) {
            double playerAverage;
            if(playerStats(player)->isAllGames()) {
                playerAverage = 0;
            }
            else{
                playerAverage = playerStats(player)->average();
            }
            if(playerAverage > average) {
                average = playerAverage;
                index = playersList.indexOf(player);
            }
        }
        result.append(QPair<QSharedPointer<Player>,double>(playersList.takeAt(index),average));
    }

    return result;
}


void League::calculateMatchdays()
{
    QList<QSharedPointer<Game> > source;
    source.append(filteredGames());

    m_calculatedGames.clear();
    m_playerStatistics.clear();

    for(int i = 0; i<source.size(); i++) {
        m_calculatedGames.append(source.at(i));
        QList<QPair<QSharedPointer<Player>,double> > players = sortPlayersAfterAverage();

        QSharedPointer<Matchday> matchday = QSharedPointer<Matchday>(new Matchday(this));
        matchday->setGame(m_calculatedGames.last());
        matchday->setNumber(i+1);

        for(int i = 0; i<players.size(); i++) {
            matchday->addPlayer(players.at(i).first);
            matchday->addAverage(players.at(i).first, players.at(i).second);
        }

        m_matchdays.append(matchday);

        if(i<source.size()-1)
            m_playerStatistics.clear();
    }

    League::currentMatchDayNumber = m_matchdays.size();
}

QList<QSharedPointer<Game> > League::games() const
{
    return m_games.resolveList();
}

QList<QSharedPointer<Game> > League::calculatedGames()
{
    if(m_calculatedGames.isEmpty()) {
        m_calculatedGames.append(calculatePossibleGames());
        currentMatchDayNumber = m_calculatedGames.size();
    }

    return m_calculatedGames;
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
        foreach(QSharedPointer<Game> game, p->games()) {
            if(!possibleGames.contains(game)) {
                possibleGames.append(game);
            }
        }
    }

    qSort(possibleGames.begin(), possibleGames.end(), sortGamesByDate);

    QList<QSharedPointer<Game> > fittingGames;
    for(int i = 0; i < possibleGames.size(); ++i) {
        QSharedPointer<Game> game = possibleGames.at(i);
        if(game->type() != Game::Doppelkopf || game->creationTime().date() < startDate() || game->creationTime().date() > endDate() || !hasEnoughPlayers(game)) {
            continue;
        }
        else{
            fittingGames.append(game);
        }
    }

    return fittingGames;
}

QList<QSharedPointer<Game> > League::filteredGames()
{
    QList<QSharedPointer<Game> > filter;

    foreach(QSharedPointer<Game> game, calculatedGames()) {
        if(game->completedPercentage() >= m_finishedGamesPercentage) {
            filter.append(game);
        }
    }

    return filter;
}

QSharedPointer<PlayerStatistics> League::playerStats(QSharedPointer<Player> player)
{
    if(m_playerStatistics.value(player) == 0) {
        QSharedPointer<PlayerStatistics> stats(new PlayerStatistics(this));
        stats->setPlayer(player.data());
        QList<QSharedPointer<Game> > gameList = filteredGames();
        foreach(QSharedPointer<Game> game, gameList) {
            if(!game->players().contains(player)) {
                gameList.removeOne(game);
            }
        }
        stats->setGames(gameList);
        m_playerStatistics.insert(player, stats);
    }

    return m_playerStatistics.value(player);
}

double League::playerRatio() const
{
    return m_playerRatio;
}

void League::setPlayerRatio(const double ratio)
{
    m_playerRatio = ratio;
}

QString League::name() const
{
    return m_name;
}

void League::setName(const QString name)
{
    m_name = name;
}

void League::recalculate()
{
    m_matchdays.clear();
    m_playerStatistics.clear();
    m_calculatedGames.clear();
}

double League::finishedGamesBorder() const
{
    return m_finishedGamesPercentage;
}

void League::setFinishedGamesBorder(double border)
{
    m_finishedGamesPercentage = border;
}

QSharedPointer<Matchday> League::currentMatchday()
{
    if(m_matchdays.isEmpty())
        calculateMatchdays();

    return m_matchdays.at(League::currentMatchDayNumber-1);
}

QList<QSharedPointer<Matchday> > League::matchdays()
{
    if(m_matchdays.isEmpty())
        calculateMatchdays();

    return m_matchdays;
}
