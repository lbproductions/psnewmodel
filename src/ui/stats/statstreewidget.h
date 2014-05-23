#ifndef STATSTREEWIDGET_H
#define STATSTREEWIDGET_H

#include <QTreeWidget>

class League;
class Game;
class Player;

class StatsTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit StatsTreeWidget(QWidget *parent = 0);

    void setLeague(QSharedPointer<League> league);

    virtual void setGames(QList<QSharedPointer<Game> > games);
    virtual void setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players) = 0;

protected:
    void highlightMax(Qt::SortOrder order, QColor backgroundColor);
    void setColumnsWidthToContent();
};

#endif // STATSTREEWIDGET_H
