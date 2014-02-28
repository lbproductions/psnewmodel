#include "livedrink.h"

#include "player.h"
#include "round.h"
#include "drink.h"



LiveDrink::LiveDrink(QObject *parent) :
    QObject(parent),
    m_time(QDateTime::currentDateTime()),
    m_player(QpRelation(&LiveDrink::player)),
    m_round(QpRelation(&LiveDrink::round)),
    m_drink(QpRelation(&LiveDrink::drink))
{
}

LiveDrink::~LiveDrink()
{
}

QSharedPointer<Player> LiveDrink::player() const
{
    return m_player;
}

void LiveDrink::setPlayer(QSharedPointer<Player> player)
{
    m_player = player;
}

QSharedPointer<Round> LiveDrink::round() const
{
    return m_round;
}

void LiveDrink::setRound(QSharedPointer<Round> round)
{
    m_round = round;
}

QSharedPointer<Drink> LiveDrink::drink() const
{
    return m_drink;
}

void LiveDrink::setDrink(QSharedPointer<Drink> drink)
{
    m_drink = drink;
}

void LiveDrink::clearRelations()
{
    setRound(QSharedPointer<Round>());
    setPlayer(QSharedPointer<Player>());
    setDrink(QSharedPointer<Drink>());
}

QDateTime LiveDrink::time() const
{
    return m_time;
}

void LiveDrink::setTime(const QDateTime &time)
{
    m_time = time;
}


