#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

namespace Ui {
class StatisticsWidget;
}

class Game;
class Player;

class PlayerStatsWidget;

class StatisticsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit StatisticsWidget(QWidget *parent = 0);
    ~StatisticsWidget();

    void setGame(QSharedPointer<Game> game);

private slots:
    void onButtonPressed();

private:
    Ui::StatisticsWidget *ui;

    QSharedPointer<Game> m_game;

    QHash<QPushButton*, QWidget*> m_statsWidgets;
    QHash<QPushButton*, QLabel*> m_splitLabels;
};

#endif // STATISTICSWIDGET_H
