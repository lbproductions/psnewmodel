#include "schmeisserei.h"

#include "player.h"
#include "round.h"



Schmeisserei::Schmeisserei(QObject *parent) :
    QObject(parent),
    m_type(UnkownType),
    m_round("round",this),
    m_player("player",this)
{
}

QSharedPointer<Player> Schmeisserei::player() const
{
    return m_player.resolve();
}

void Schmeisserei::setPlayer(const QSharedPointer<Player> &player)
{
    m_player.relate(player);
}

int Schmeisserei::_type() const
{
    return static_cast<int>(type());
}

void Schmeisserei::_setType(int type)
{
    setType(static_cast<Type>(type));
}

QSharedPointer<Round> Schmeisserei::round() const
{
    return m_round.resolve();
}

void Schmeisserei::setRound(const QSharedPointer<Round> &round)
{
    m_round.relate(round);
}

Schmeisserei::Type Schmeisserei::type() const
{
    return m_type;
}

void Schmeisserei::setType(const Type &type)
{
    m_type = type;
}


