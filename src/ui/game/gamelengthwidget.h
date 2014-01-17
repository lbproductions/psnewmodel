#ifndef GAMELENGTHWIDGET_H
#define GAMELENGTHWIDGET_H

#include <QWidget>
#include <data/game.h>

class GameLengthWidget : public QWidget
{
    Q_OBJECT
public:
    enum Context{
        TotalTime,
        Duration
    };

    explicit GameLengthWidget(QWidget *parent = 0);

    void setGame(QSharedPointer<Game> _game);

signals:

public slots:

private slots:
    void update();

private:
    void mouseReleaseEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent *event);
    QSize sizeHint() const;

    QSharedPointer<Game> m_game;

    Context m_context;

};

#endif // GAMELENGTHWIDGET_H
