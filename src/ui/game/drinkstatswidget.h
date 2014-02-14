#ifndef DRINKSTATSWIDGET_H
#define DRINKSTATSWIDGET_H

#include <QWidget>

class Game;

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

private:
    Ui::DrinkStatsWidget *ui;
};

#endif // DRINKSTATSWIDGET_H
