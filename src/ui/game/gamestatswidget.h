#ifndef GAMESTATSWIDGET_H
#define GAMESTATSWIDGET_H

#include <QWidget>

namespace Ui {
    class GameStatsWidget;
}

class Game;

class GameStatsWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit GameStatsWidget(QWidget *parent = 0);
        ~GameStatsWidget();

        void setGame(QSharedPointer<Game> game);

   private slots:
        void update();
        void togglePredictedTime();
        void toggleWeightedAverageRoundTime();
        
    private:
        Ui::GameStatsWidget *ui;

        QSharedPointer<Game> m_game;
        bool m_predictedTimeLeftShown;
        bool m_roundAverageWeighted;
};

#endif // GAMESTATSWIDGET_H
