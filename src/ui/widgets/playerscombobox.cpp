#include "playerscombobox.h"

#include <data/player.h>

PlayersComboBox::PlayersComboBox(QWidget *parent) :
    QComboBox(parent)
{
    addItem(tr("---"));
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
    return m_players.contains(player);
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
    addItem(QIcon(player->avatar()), player->name());
}

void PlayersComboBox::setCurrentPlayer(QSharedPointer<Player> player)
{
    if(!contains(player))
        return;

    setCurrentIndex(findText(player->name()));
}

void PlayersComboBox::removePlayer(QSharedPointer<Player> player)
{
    removeItem(findText(player->name()));
    m_players.removeAll(player);
}
