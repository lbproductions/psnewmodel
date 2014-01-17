#include "livedrink.h"

#include "player.h"
#include "round.h"
#include "drink.h"



LiveDrink::LiveDrink(QObject *parent) :
    QObject(parent),
    m_time(QDateTime::currentDateTime()),
    m_player("player",this),
    m_round("round",this),
    m_drink("drink",this)
{
}

LiveDrink::~LiveDrink()
{
}

QSharedPointer<Player> LiveDrink::player() const
{
    return m_player.resolve();
}

void LiveDrink::setPlayer(QSharedPointer<Player> player)
{
    QSharedPointer<LiveDrink> sharedThis = Qp::sharedFrom(this);

    QSharedPointer<Player> p = this->player();
    if(p) {
        p->removeLiveDrink(sharedThis);
    }

    m_player.relate(player);
    if(player) {
        player->addLiveDrink(sharedThis);
    }

}

QSharedPointer<Round> LiveDrink::round() const
{
    return m_round.resolve();
}

void LiveDrink::setRound(QSharedPointer<Round> round)
{
    QSharedPointer<LiveDrink> sharedThis = Qp::sharedFrom(this);

    QSharedPointer<Round> r = this->round();
    if(r) {
        r->removeDrink(sharedThis);
    }

    m_round.relate(round);
    if(round) {
        round->addDrink(sharedThis);
    }
}

QSharedPointer<Drink> LiveDrink::drink() const
{
    return m_drink.resolve();
}

void LiveDrink::setDrink(QSharedPointer<Drink> drink)
{
    QSharedPointer<LiveDrink> sharedThis = Qp::sharedFrom(this);

    QSharedPointer<Drink> d = this->drink();
    if(d) {
        d->removeLiveDrink(sharedThis);
    }

    m_drink.relate(drink);
    if(drink) {
        drink->addLiveDrink(sharedThis);
    }
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


