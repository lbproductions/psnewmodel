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
class ResumeWidget;

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

    void enableActionsBasedOnState();

    void on_pushButtonAddPlayers_clicked();
    void addPlayerToGame(QSharedPointer<Player> player);
    void on_buttonBox_accepted();

    void on_actionPlayPause_triggered();
    void on_actionStop_Game_triggered();

    void on_actionAdd_round_triggered();
    void on_actionAdd_drinks_triggered();
    void on_actionAdd_schmeisserei_triggered();
    void on_actionStats_triggered();
    void on_actionSettings_triggered();
    void on_TableViewOverviewDoubleClicked(const QModelIndex &index);
    void on_toolButtonSetComment_clicked();

    void onNewRoundStarted();
    void updateTimes();
    void updateSizes();

private:
    QWidget *popupWidget() const;
    void setPopupWidget(QWidget *popupWidget);

    Ui::GameWindow *ui;

    QSharedPointer<Game> m_game;

    GameOverviewModel *m_gameOverViewModel;
    GameInformationModel *m_informationModel;
    OverviewPlayerHeaderView *m_verticalHeaderView;

    QPointer<QWidget> m_popupWidget;
    ResumeWidget* m_resumeWidget;
};

#endif // GAMEWINDOW_H
