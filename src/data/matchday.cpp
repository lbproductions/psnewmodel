#include "matchday.h"

#include "league.h"
#include "game.h"
#include "player.h"

Matchday::Matchday(QObject *parent) :
    QObject(parent)
{
}

Matchday::~Matchday()
{
    m_playersSortedByAverage.clear();
    m_averages.clear();
    m_game.clear();
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
    int placement = 1;
    foreach(QSharedPointer<Player> otherPlayer, m_playersSortedByAverage) {
        if(player->name() != otherPlayer->name() && m_averages.value(otherPlayer) > m_averages.value(player)) {
            placement++;
        }
    }
    return placement;
}

void Matchday::setNumber(int number)
{
    m_number = number;
}

int Matchday::number()
{
    return m_number;
}

