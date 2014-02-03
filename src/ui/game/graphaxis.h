#ifndef GRAPHAXIS_H
#define GRAPHAXIS_H

#include <QWidget>

class GraphWidget;


class GraphAxis : public QWidget
{
    Q_OBJECT
public:
    explicit GraphAxis(QWidget *parent = 0);

    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;

    void setGraph(GraphWidget *graph);

private:
    GraphWidget *m_graph;
};

#endif // GRAPHAXIS_H
