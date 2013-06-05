#ifndef PLAYERSCOMBOBOX_H
#define PLAYERSCOMBOBOX_H

#include <QComboBox>

class Player;

class PlayersComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit PlayersComboBox(QWidget *parent = 0);

    void clear();

    QList<QSharedPointer<Player> > players() const;
    QSharedPointer<Player> currentPlayer() const;
    bool contains(QSharedPointer<Player> player) const;

    int indexOf(QSharedPointer<Player> player) const;

    void setPlayers(const QList<QSharedPointer<Player> > &players);
    void addPlayers(const QList<QSharedPointer<Player> > &players);
    void addPlayer(QSharedPointer<Player> player);
    void setCurrentPlayer(QSharedPointer<Player> player);

    void removePlayer(QSharedPointer<Player> player);
private:
    QList<QSharedPointer<Player> > m_players;
    QMultiHash<QSharedPointer<Player>, int> m_indexes;
};

#endif // PLAYERSCOMBOBOX_H
