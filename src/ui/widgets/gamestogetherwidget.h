#ifndef GAMESTOGETHERWIDGET_H
#define GAMESTOGETHERWIDGET_H

#include <QWidget>
#include <QTreeWidget>

class Game;
class Player;

class GamesTogetherWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit GamesTogetherWidget(QWidget *parent = 0);
    ~GamesTogetherWidget();

    void setData(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players);

private slots:
    void update();

private:
    QList<QSharedPointer<Game> > m_games;
    QList<QSharedPointer<Player> > m_players;

    QHash<QString,QTreeWidgetItem*> m_items;
};

#endif // GAMESTOGETHERWIDGET_H
