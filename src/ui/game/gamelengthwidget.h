#ifndef GAMELENGTHWIDGET_H
#define GAMELENGTHWIDGET_H

#include <QWidget>
#include <data/game.h>

class GameLengthWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameLengthWidget(QWidget *parent = 0);

    void setGame(QSharedPointer<Game> _game);

signals:

public slots:

private slots:
    void update();

private:
    void paintEvent(QPaintEvent *event);
    QSize sizeHint() const;

    QSharedPointer<Game> m_game;

};

#endif // GAMELENGTHWIDGET_H
