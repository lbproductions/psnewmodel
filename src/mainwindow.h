#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionPlayers_triggered();
    void on_actionGames_triggered();
    void on_actionPlaces_triggered();
    void on_actionDrinks_triggered();

    void restoreWindowState();

private:
    Ui::MainWindow *ui;

    void saveWindowState();
};

#endif // MAINWINDOW_H
