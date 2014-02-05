#include "graphaxis.h"

#include "ui/widgets/graphwidget.h"

#include <QPainter>
#include <QDebug>
#include <QPaintEvent>

GraphAxis::GraphAxis(QWidget *parent) :
    QWidget(parent),
    m_graph(nullptr)
{
}

void GraphAxis::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    QRect rect = e->rect();

    painter.setPen(Qt::transparent);
    painter.setBrush(palette().base().color());
    painter.drawRect(rect);

    QPen pen(palette().highlight().color());
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(rect.topRight(),rect.bottomRight());

    if(!m_graph)
        return;

    int y = m_graph->minY();

    y /= 10;
    y -= 1;
    y *= 10;

    QTextOption to;
    to.setAlignment(Qt::AlignRight);

    for(; y < m_graph->maxY(); y += 10) {
        if((y/10) % 5 == 0) {
            pen.setWidth(2);
            pen.setColor(palette().highlightedText().color());
            painter.setPen(pen);
        }
        else {
            pen.setWidth(1);
            pen.setColor(palette().highlight().color());
            painter.setPen(pen);
        }
        painter.drawText(QRect(QPoint(width() - 35, m_graph->translateY(y) - 8),
                               QSize(30,20)),
                         QString::number(y), to);
    }
}

void GraphAxis::setGraph(GraphWidget *graph)
{
    m_graph = graph;

    connect(graph, &GraphWidget::updated, [=]{repaint();});
}
