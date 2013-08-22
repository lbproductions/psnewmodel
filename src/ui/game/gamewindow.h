#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>

#include <QPointer>

namespace Ui {
class GameWindow;
}

class Game;
class GameOverviewModel;
class Player;
class PopupWidget;
class GameInformationModel;
class OverviewPlayerHeaderView;
class StatisticsWidget;

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

    void resizeEvent(QResizeEvent *);

private slots:
    void on_actionPlayPause_triggered();

    void enableActionsBasedOnState();

    void on_actionAdd_round_triggered();
    void onNewRoundStarted();

    void on_actionAdd_schmeisserei_triggered();

    void on_actionToggleSidebar_triggered();
    void setSidebarToggleActionBasedUponSidebarState();
    void setSidebarToggleToHide();
    void setSidebarToggleToShow();

    void updateTimes();

    void on_pushButtonAddPlayers_clicked();

    void addPlayerToGame(QSharedPointer<Player> player);

    PopupWidget *popupWidget() const;
    void setPopupWidget(PopupWidget *popupWidget);

    void on_buttonBox_accepted();

    void on_actionAdd_drinks_triggered();

private:
    Ui::GameWindow *ui;

    QSharedPointer<Game> m_game;
    GameOverviewModel *m_gameOverViewModel;
    GameInformationModel *m_informationModel;

    QPointer<PopupWidget> m_popupWidget;

    OverviewPlayerHeaderView *m_verticalHeaderView;
    StatisticsWidget* m_statsWidget;
};

#endif // GAMEWINDOW_H
