#ifndef DRINKSTATSWIDGET_H
#define DRINKSTATSWIDGET_H

#include <QWidget>

class Game;
class League;
class Player;

namespace Ui {
class DrinkStatsWidget;
}

class DrinkStatsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrinkStatsWidget(QWidget *parent = 0);
    ~DrinkStatsWidget();

    void setGames(QList<QSharedPointer<Game> > games);
    void setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players);
    void setLeague(QSharedPointer<League> league);

private:
    Ui::DrinkStatsWidget *ui;
};

#endif // DRINKSTATSWIDGET_H
