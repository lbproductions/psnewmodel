#include "matchday.h"

#include "league.h"
#include "game.h"
#include "player.h"

Matchday::Matchday(QObject *parent) :
    QObject(parent)
{
}

void Matchday::setLeague(QSharedPointer<League> league)
{
    m_league = league;
}

void Matchday::setGame(QSharedPointer<Game> game)
{
    m_game = game;
}

QSharedPointer<Game> Matchday::game()
{
    return m_game;
}

QList<QSharedPointer<Player> > Matchday::playersSortedByAverage()
{
    return m_playersSortedByAverage;
}

void Matchday::setPlayersSortedByAverage(QList<QSharedPointer<Player> > list)
{
    m_playersSortedByAverage = list;
}

void Matchday::addPlayer(QSharedPointer<Player> player)
{
    m_playersSortedByAverage.append(player);
}

double Matchday::average(QSharedPointer<Player> player)
{
    return m_averages.value(player);
}

void Matchday::addAverage(QSharedPointer<Player> player, double average)
{
    m_averages.insert(player, average);
}

int Matchday::placementOfPlayer(QSharedPointer<Player> player)
{
    return m_playersSortedByAverage.indexOf(player)+1;
}

void Matchday::setNumber(int number)
{
    m_number = number;
}

int Matchday::number()
{
    return m_number;
}

