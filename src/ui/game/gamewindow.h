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
class DialogController;

class GameWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

    void setGame(const QSharedPointer<Game> &game);

protected:
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
    void onTableViewOverviewDoubleClicked(const QModelIndex &index);

    void onNewRoundStarted();
    void updateSizes();

    void on_actionAdd_Hochzeit_triggered();

    void on_actionAdd_Solo_triggered();

    void on_actionAdd_Trumpfabgabe_triggered();

    void onDialogClosed();

    void on_actionSet_comment_triggered();

private:
    Ui::GameWindow *ui;

    QSharedPointer<Game> m_game;

    GameOverviewModel *m_gameOverViewModel;
    GameInformationModel *m_informationModel;
    OverviewPlayerHeaderView *m_verticalHeaderView;

    ResumeWidget* m_resumeWidget;
    DialogController *m_dialogController;

    int m_minimumColumnWidth;
};

#endif // GAMEWINDOW_H
