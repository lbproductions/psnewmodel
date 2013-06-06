#include "playerslistwidget.h"

#include <data/player.h>

PlayersListWidget::PlayersListWidget(QWidget *parent) :
    QListWidget(parent)
{
//    connect(this, &QListWidget::clicked,
//            this, &PlayersListWidget::indexActivated);
    connect(this, &QListWidget::activated,
            this, &PlayersListWidget::indexActivated);
}

void PlayersListWidget::clear()
{
    m_players.clear();
    m_indexes.clear();
    QListWidget::clear();
}

QList<QSharedPointer<Player> > PlayersListWidget::players() const
{
    return m_players;
}

QSharedPointer<Player> PlayersListWidget::currentPlayer() const
{
    int index = currentIndex().row();
    if(index < 0 || index >= m_players.size())
        return QSharedPointer<Player>();

    return m_players.at(index);
}

bool PlayersListWidget::contains(QSharedPointer<Player> player) const
{
    return m_indexes.contains(player);
}

int PlayersListWidget::indexOf(QSharedPointer<Player> player) const
{
    return m_indexes.value(player, -1);
}

void PlayersListWidget::setPlayers(const QList<QSharedPointer<Player> > &players)
{
    clear();
    addPlayers(players);
}

void PlayersListWidget::addPlayers(const QList<QSharedPointer<Player> > &players)
{
    foreach(QSharedPointer<Player> p, players)
        addPlayer(p);
}

void PlayersListWidget::addPlayer(QSharedPointer<Player> player)
{
    if(!player || contains(player))
        return;

    m_indexes.insert(player, m_players.size());
    m_players.append(player);
    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(player->name());
    QPixmap pm(16,16);
    pm.fill(player->color());
    newItem->setIcon(QIcon(pm));
    insertItem(m_players.size(), newItem);
}

void PlayersListWidget::setCurrentPlayer(QSharedPointer<Player> player)
{
    if(!contains(player))
        return;

    setCurrentRow(indexOf(player));
}

void PlayersListWidget::removePlayer(QSharedPointer<Player> player)
{
    while(contains(player)) {
        int index = indexOf(player);
        delete takeItem(index);
        m_indexes.remove(player, index);
    }

    m_indexes.clear();
    m_players.removeAll(player);
    for(int i = 0; i < m_players.size(); ++i) {
        m_indexes.insert(m_players.at(i), i);
    }
}

void PlayersListWidget::indexActivated(const QModelIndex &)
{
    QSharedPointer<Player> player = currentPlayer();
    if(player)
        emit playerActivated(player);
}
