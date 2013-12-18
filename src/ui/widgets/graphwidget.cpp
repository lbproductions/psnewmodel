#include "graphwidget.h"

GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent),
    m_maxY(std::numeric_limits<int>::min()),
    m_minY(std::numeric_limits<int>::max()),
    m_cellWidth(70)
{
}


int GraphWidget::maxY() const
{
    return m_maxY;
}

void GraphWidget::setMaxY(int maxY)
{
    m_maxY = maxY;
}

int GraphWidget::minY() const
{
    return m_minY;
}

void GraphWidget::setMinY(int minY)
{
    m_minY = minY;
}

int GraphWidget::originX() const
{
    return 0;
}

int GraphWidget::originY() const
{
    return qAbs(maxY()) * pixelsPerPoint();
}

int GraphWidget::cellWidth() const
{
    return m_cellWidth;
}

void GraphWidget::setCellWidth(int width)
{
    m_cellWidth = width;
}

int GraphWidget::translateX(int x) const
{
    return x * 40 - 1;
}

int GraphWidget::translateY(int y) const
{
    return -pixelsPerPoint() * y + originY();
}

double GraphWidget::pixelsPerPoint() const
{
    int range = maxY() - minY();
    return (double) height() / (double) range;
}

