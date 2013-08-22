#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

namespace Ui {
class StatisticsWidget;
}

class Game;
class Player;

class StatisticsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit StatisticsWidget(QWidget *parent = 0);
    ~StatisticsWidget();

    void setGame(QSharedPointer<Game> game);

    QString getDrinkString(QSharedPointer<Player> player);

private slots:
    void update();
    
private:
    Ui::StatisticsWidget *ui;

    QSharedPointer<Game> m_game;

    QVBoxLayout* m_drinkLayout;
    QHash<QSharedPointer<Player>, QLabel*> m_drinkLabels;
};

#endif // STATISTICSWIDGET_H
