#ifndef GAMESTOGETHERWIDGET_H
#define GAMESTOGETHERWIDGET_H

#include "statstreewidget.h"


class Game;
class Player;

class GamesTogetherWidget : public StatsTreeWidget
{
    Q_OBJECT

public:
    explicit GamesTogetherWidget(QWidget *parent = 0);
    ~GamesTogetherWidget();

    virtual void setGames(QList<QSharedPointer<Game> > list, QList<QSharedPointer<Player> > players);

private slots:
    void update();

private:
    QList<QSharedPointer<Game> > m_games;
    QList<QSharedPointer<Player> > m_players;

    QHash<QString,QTreeWidgetItem*> m_items;
};

#endif // GAMESTOGETHERWIDGET_H
