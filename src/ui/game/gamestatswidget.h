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
        
    private:
        Ui::GameStatsWidget *ui;

        QSharedPointer<Game> m_game;
};

#endif // GAMESTATSWIDGET_H
