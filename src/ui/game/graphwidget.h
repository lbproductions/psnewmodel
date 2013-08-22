#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>


class Game;

class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);

    QSize sizeHint() const;

    QSharedPointer<Game> game() const;
    void setGame(const QSharedPointer<Game> &game);

    int originX() const;
    int originY() const;

    int translateX(int x) const;
    int translateY(int y) const;

    double pixelsPerPoint() const;

    int maxY() const;
    void setMaxY(int maxY);

    int minY() const;
    void setMinY(int minY);

public slots:
    void updateGraphs();

signals:
    void graphUpdated();

private:
    void paintEvent(QPaintEvent *event);

    int m_maxY;
    int m_minY;

    QList<QList<int> > m_rounds;

    QSharedPointer<Game> m_game;
};

#endif // GRAPH_H
