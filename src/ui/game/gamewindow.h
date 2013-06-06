#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class GameWindow;
}

class Game;
class GameOverviewModel;
class Player;

class GameWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();
    
    QSharedPointer<Game> game() const;
    void setGame(const QSharedPointer<Game> &game);

    void wheelEvent(QWheelEvent *e);

    void mousePressEvent(QMouseEvent *e);

private slots:
    void on_actionPlayPause_triggered();

    void enableActionsBasedOnState();

    void on_actionAdd_round_triggered();

    void on_actionAdd_schmeisserei_triggered();

    void on_actionToggleSidebar_triggered();
    void setSidebarToggleActionBasedUponSidebarState();
    void setSidebarToggleToHide();
    void setSidebarToggleToShow();

    void updateTimes();

    void on_pushButtonAddPlayers_clicked();

    void addPlayerToGame(QSharedPointer<Player> player);

    QWidget *extraWidget() const;
    void setExtraWidget(QWidget *extraWidget);

    void on_buttonBox_accepted();

private:
    Ui::GameWindow *ui;

    QSharedPointer<Game> m_game;
    GameOverviewModel *m_gameOverViewModel;

    QWidget *m_extraWidget;
};

#endif // GAMEWINDOW_H
