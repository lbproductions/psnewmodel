#include "recontrastatslegendwidget.h"

#include <QPainter>

ReContraStatsLegendWidget::ReContraStatsLegendWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedHeight(100);
}


void ReContraStatsLegendWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int height = 20;
    int startHeight = 20;

    painter.drawText(QRect(0,startHeight,this->width(), height), Qt::AlignCenter, tr("RePercentage"));
    startHeight+=height;
    painter.drawText(QRect(0,startHeight,this->width(), height), Qt::AlignCenter, tr("ReWinPercentage"));
    startHeight+=height;
    painter.drawText(QRect(0,startHeight,this->width(), height), Qt::AlignCenter, tr("ContraWinPercentage"));
}
