#include "old_offlineGameInformation.h"

#include "game.h"
#include "player.h"

OLD_OfflineGameInformation::OLD_OfflineGameInformation(QObject *parent):
    QObject(parent),
    m_game("game", this),
    m_player("player", this)
{
}

QSharedPointer<Game> OLD_OfflineGameInformation::game() const
{
    return m_game.resolve();
}

void OLD_OfflineGameInformation::setGame(QSharedPointer<Game> game)
{
    m_game.clear();
    m_game.relate(game);
}

QSharedPointer<Player> OLD_OfflineGameInformation::player() const
{
    return m_player.resolve();
}

void OLD_OfflineGameInformation::setPlayer(QSharedPointer<Player> player)
{
    m_player.clear();
    m_player.relate(player);
}

int OLD_OfflineGameInformation::hochzeiten() const
{
    return m_hochzeiten;
}

void OLD_OfflineGameInformation::setHochzeiten(const int hochzeiten)
{
    m_hochzeiten = hochzeiten;
}

int OLD_OfflineGameInformation::soli() const
{
    return m_soli;
}

void OLD_OfflineGameInformation::setSoli(const int soli)
{
    m_soli = soli;
}

int OLD_OfflineGameInformation::trumpfabgaben() const
{
    return m_trumpfabgaben;
}

void OLD_OfflineGameInformation::setTrumpfabgaben(const int trumpfabgaben)
{
    m_trumpfabgaben = trumpfabgaben;
}

int OLD_OfflineGameInformation::reRounds() const
{
    return m_reRounds;
}

void OLD_OfflineGameInformation::setReRounds(const int reRounds)
{
    m_reRounds = reRounds;
}

int OLD_OfflineGameInformation::reWins() const
{
    return m_reWins;
}

void OLD_OfflineGameInformation::setReWins(const int reWins)
{
    m_reWins = reWins;
}

int OLD_OfflineGameInformation::contraRounds() const
{
    return m_contraRounds;
}

void OLD_OfflineGameInformation::setContraRounds(const int contraRounds)
{
    m_contraRounds = contraRounds;
}

int OLD_OfflineGameInformation::contraWins() const
{
    return m_contraWins;
}

void OLD_OfflineGameInformation::setContraWins(const int contraWins)
{
    m_contraWins = contraWins;
}

int OLD_OfflineGameInformation::placement() const
{
    return m_placement;
}

void OLD_OfflineGameInformation::setPlacement(const int placement)
{
    m_placement = placement;
}

int OLD_OfflineGameInformation::points() const
{
    return m_points;
}

void OLD_OfflineGameInformation::setPoints(const int points)
{
    m_points = points;
}
