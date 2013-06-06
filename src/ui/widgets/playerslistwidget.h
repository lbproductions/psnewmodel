#ifndef PLAYERSLISTWIDGET_H
#define PLAYERSLISTWIDGET_H

#include <QListWidget>

class Player;

class PlayersListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit PlayersListWidget(QWidget *parent = 0);

    QList<QSharedPointer<Player> > players() const;
    void setPlayers(const QList<QSharedPointer<Player> > &players);

    void clear();
    QSharedPointer<Player> currentPlayer() const;

    bool contains(QSharedPointer<Player> player) const;
    int indexOf(QSharedPointer<Player> player) const;
    void addPlayers(const QList<QSharedPointer<Player> > &players);
    void addPlayer(QSharedPointer<Player> player);

    void setCurrentPlayer(QSharedPointer<Player> player);
    void removePlayer(QSharedPointer<Player> player);

signals:
    void playerActivated(QSharedPointer<Player> player);

private slots:
    void indexActivated(const QModelIndex &index);

private:
    QList<QSharedPointer<Player> > m_players;
    QMultiHash<QSharedPointer<Player>, int> m_indexes;

};

#endif // PLAYERSLISTWIDGET_H
