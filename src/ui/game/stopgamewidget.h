#ifndef STOPGAMEWIDGET_H
#define STOPGAMEWIDGET_H

#include <QWidget>

namespace Ui {
class StopGameWidget;
}

class Game;

class StopGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StopGameWidget(QWidget *parent = 0);
    ~StopGameWidget();

    void setGame(QSharedPointer<Game> game);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::StopGameWidget *ui;

    QSharedPointer<Game> m_game;
};

#endif // STOPGAMEWIDGET_H
