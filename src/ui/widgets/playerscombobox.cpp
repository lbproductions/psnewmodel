#include "playerscombobox.h"

#include <data/player.h>

PlayersComboBox::PlayersComboBox(QWidget *parent) :
    QComboBox(parent)
{
    addItem(tr("---"));
}

void PlayersComboBox::clear()
{
    m_players.clear();
    m_indexes.clear();
    QComboBox::clear();
}

QList<QSharedPointer<Player> > PlayersComboBox::players() const
{
    return m_players;
}

QSharedPointer<Player> PlayersComboBox::currentPlayer() const
{
    int index = currentIndex() - 1;
    if(index < 0 || index >= m_players.size())
        return QSharedPointer<Player>();

    return m_players.at(index);
}

bool PlayersComboBox::contains(QSharedPointer<Player> player) const
{
    return m_indexes.contains(player);
}

int PlayersComboBox::indexOf(QSharedPointer<Player> player) const
{
    return m_indexes.value(player, -1);
}

void PlayersComboBox::setPlayers(const QList<QSharedPointer<Player> > &players)
{
    clear();
    addPlayers(players);
}

void PlayersComboBox::addPlayers(const QList<QSharedPointer<Player> > &players)
{
    foreach(QSharedPointer<Player> p, players)
        addPlayer(p);
}

void PlayersComboBox::addPlayer(QSharedPointer<Player> player)
{
    if(!player || contains(player))
        return;

    m_players.append(player);
    m_indexes.insert(player, m_players.size());
    addItem(QIcon(player->avatar()),
            player->name());
}

void PlayersComboBox::setCurrentPlayer(QSharedPointer<Player> player)
{
    if(!contains(player))
        return;

    setCurrentIndex(indexOf(player));
}

void PlayersComboBox::removePlayer(QSharedPointer<Player> player)
{
    while(contains(player)) {
        int index = indexOf(player);
        removeItem(index);
        m_indexes.remove(player, index);
    }

    m_players.removeAll(player);
}
