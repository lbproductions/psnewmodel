#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>

class MainWindow;

namespace Ui {
class StartWidget;
}

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(MainWindow *parent = 0);
    ~StartWidget();

    void init(MainWindow *window);

private slots:
    void on_listViewGames_doubleClicked(const QModelIndex &index);

    void on_listViewLeague_doubleClicked(const QModelIndex &index);

private:
    Ui::StartWidget *ui;
};

#endif // STARTWIDGET_H
