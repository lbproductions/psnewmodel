#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class PlayersListModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void closeEvent(QCloseEvent *event);
private slots:
    void on_actionNew_Game_triggered();

    void on_actionPlayers_triggered();
    void on_actionGames_triggered();
    void on_actionPlaces_triggered();
    void on_actionDrinks_triggered();

    void restoreWindowState();

    void on_actionPlayerInformation_triggered();
    void on_actionDrinkInformation_triggered();

    void on_treeViewGames_doubleClicked(const QModelIndex &index);

    void on_actionOpen_library_triggered();

private:
    Ui::MainWindow *ui;
    PlayersListModel* m_playerListModel;

    void saveWindowState();
};

#endif // MAINWINDOW_H
