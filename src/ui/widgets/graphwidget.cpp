#include "graphwidget.h"

GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent),
    m_maxY(std::numeric_limits<int>::min()),
    m_minY(std::numeric_limits<int>::max()),
    m_columnWidth(40)
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

int GraphWidget::columnWidth() const
{
    return m_columnWidth;
}

void GraphWidget::setColumnWidth(int width)
{
    m_columnWidth = width;
    update();
}

int GraphWidget::translateX(int x) const
{
    return x * m_columnWidth - 1;
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

