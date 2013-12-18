#ifndef GRAPH_H
#define GRAPH_H

#include <ui/widgets/graphwidget.h>

class Game;

class GameGraphWidget : public GraphWidget
{
    Q_OBJECT
public:
    explicit GameGraphWidget(QWidget *parent = 0);

    QSize sizeHint() const;

    QSharedPointer<Game> game() const;
    void setGame(const QSharedPointer<Game> &game);

public slots:
    void updateGraphs();

signals:
    void graphUpdated();

private:
    void paintEvent(QPaintEvent *event);

    QList<QList<int> > m_rounds;

    QSharedPointer<Game> m_game;
};

#endif // GRAPH_H
