#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class GameWindow;
}

class Game;
class GameOverviewModel;

class GameWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();
    
    QSharedPointer<Game> game() const;
    void setGame(const QSharedPointer<Game> &game);

    void wheelEvent(QWheelEvent *e);

private:
    Ui::GameWindow *ui;

    QSharedPointer<Game> m_game;
    GameOverviewModel *m_gameOverViewModel;
};

#endif // GAMEWINDOW_H
