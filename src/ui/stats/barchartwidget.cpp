#include "barchartwidget.h"

#include <QPainter>

#include <data/player.h>

BarChartWidget::BarChartWidget(QWidget *parent) :
    QWidget(parent),
    m_sortingOrder(Descending),
    m_min(-1),
    m_max(-1),
    m_barDistance(0),
    m_barWidth(-1),
    m_paddingLeft(0),
    m_paddingRight(0),
    m_paddingTop(0),
    m_paddingBottom(0)
{
    this->setMinimumHeight(200);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}


void BarChartWidget::paintEvent(QPaintEvent *)
{
    if(m_values.isEmpty()) {
        return;
    }

    QPainter painter(this);
    painter.save();

    //painter.drawRect(0,0,width(),height());
    //painter.drawRect(m_paddingLeft,m_paddingTop,width()-m_paddingLeft-m_paddingRight,height()-m_paddingTop-m_paddingBottom);

    QList<QSharedPointer<Player>> players = sortPlayers();
    calcMinMax();

    int textHeight = 20;
    int titleHeight = 30;

    int totalWidth = width() - m_barDistance * (players.size()-1) - m_paddingLeft - m_paddingRight;
    int barWidth = totalWidth/players.size();
    if(m_barWidth != -1) {
        barWidth = m_barWidth;
    }
    int startHeight = height() - m_paddingBottom - textHeight;
    int totalHeight = startHeight - m_paddingTop - textHeight - titleHeight;
    if(!m_title.isEmpty()) {
        totalHeight-=textHeight;
    }

    painter.save();
    QFont font = painter.font();
    font.setPixelSize(20);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(QRect(m_paddingLeft,m_paddingTop,width()-m_paddingLeft-m_paddingRight,titleHeight), Qt::AlignLeft, m_title);
    painter.restore();

    for(int i = 0; i<players.size(); i++) {
        int y = (m_values.value(players.at(i)) / m_max) * totalHeight;
        painter.setBrush(players.at(i)->color());
        painter.setPen(QColor(108,108,108));
        painter.drawRect(m_paddingLeft + i*(barWidth+m_barDistance), startHeight-y, barWidth, y);

        painter.save();
        painter.setPen(QColor(228,228,228));
        painter.drawText(QRect(m_paddingLeft + i*(barWidth+m_barDistance), startHeight-y-textHeight, barWidth, textHeight), Qt::AlignCenter,
                         m_valueFormatString.arg(m_values.value(players.at(i))));

        painter.drawText(QRect(m_paddingLeft + i*(barWidth+m_barDistance), startHeight, barWidth, textHeight), Qt::AlignCenter,
                         players.at(i)->name());
        painter.restore();
    }

    painter.restore();
}

void BarChartWidget::setValue(QSharedPointer<Player> player, double value)
{
    m_values.insert(player, value);
}

void BarChartWidget::setSortingOrder(BarChartWidget::SortingOrder order)
{
    m_sortingOrder = order;
}

void BarChartWidget::setMinMax(double min, double max)
{
    m_min = min;
    m_max = max;
}

void BarChartWidget::setBarDistance(int distance)
{
    m_barDistance = distance;
}

void BarChartWidget::setBarWidth(int barWidth)
{
    m_barWidth = barWidth;
}

void BarChartWidget::setPadding(int left, int right, int top, int bottom)
{
    m_paddingLeft = left;
    m_paddingRight = right;
    m_paddingTop = top;
    m_paddingBottom = bottom;
}

void BarChartWidget::setValueFormatString(QString format)
{
    m_valueFormatString = format;
}

void BarChartWidget::setTitle(QString title)
{
    m_title = title;
}

QList<QSharedPointer<Player>> BarChartWidget::sortPlayers()
{
    QList<QSharedPointer<Player>> result;

    if(m_values.isEmpty()) {
        return result;
    }

    QList<double> values = m_values.values();
    switch (m_sortingOrder) {
    case BarChartWidget::Ascending:
        qSort(values.begin(), values.end(), qLess<double>());
        break;
    case BarChartWidget::Descending:
        qSort(values.begin(), values.end(), qGreater<double>());
        break;
    }

    foreach(double value, values) {
        foreach(QSharedPointer<Player> player, m_values.keys(value)) {
            if(!result.contains(player)) {
                result.append(player);
            }
        }
    }

    return result;
}

void BarChartWidget::calcMinMax()
{
    if(m_values.isEmpty()) {
        return;
    }

    QList<double> values = m_values.values();
    qSort(values.begin(), values.end(), qLess<double>());

    if(m_min == -1) {
        m_min = values.first();
    }

    if(m_max == -1) {
        m_max = values.last();
    }
}
