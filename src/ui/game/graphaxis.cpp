#include "graphaxis.h"

#include "graphwidget.h"

#include <QPainter>

GraphAxis::GraphAxis(QWidget *parent) :
    QWidget(parent)
{
}

void GraphAxis::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);

    painter.setPen(QPen(QColor(0,0,0,0)));
    painter.setBrush(palette().color(QPalette::Base));
    painter.drawRect(QRect(0,0,width(),height()));

    QPen pen(palette().highlight().color());
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(QPointF(width() - 1, 0), QPointF(width() - 1, height()));

    int y = m_graph->minY();
    y /= 10;
    y += 1;
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
                               QSize(30,20))
                         , QString::number(y), to);
    }
}

GraphWidget *GraphAxis::graph() const
{
    return m_graph;
}

void GraphAxis::setGraph(GraphWidget *graph)
{
    m_graph = graph;
}
