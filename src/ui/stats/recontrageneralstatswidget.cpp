#include "recontrageneralstatswidget.h"

#include <QPainter>

ReContraGeneralStatsWidget::ReContraGeneralStatsWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedHeight(75);
    this->setMinimumWidth(400);
}

void ReContraGeneralStatsWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int midX = this->width() / 2;

    painter.save();
    QFont font = painter.font();
    font.setPixelSize(30);
    painter.setFont(font);

    painter.drawText(QRect(0,0, 50, this->height()),  Qt::AlignLeft | Qt::AlignVCenter, tr("Re"));
    painter.drawText(QRect(0,0, this->width(), this->height()), Qt::AlignRight | Qt::AlignVCenter, tr("Contra"));

    int width = 200;
    int height = 30;

    painter.drawText(QRect(midX-width/2, 0, width, height), Qt::AlignCenter, QString::number(m_reRoundWins) + " : " + QString::number(m_contraRoundWins));
    painter.drawText(QRect(midX-width/2, height + height/2, width, height), Qt::AlignCenter, QString::number(m_reGamePointAverage) + " : " + QString::number(m_contraGamePointAverage));
    painter.restore();

    painter.drawText(QRect(midX-width/2-100, 0, width, height), Qt::AlignVCenter | Qt::AlignLeft, tr("Rounds"));
    painter.drawText(QRect(midX-width/2-100, height + height/2, width, height), Qt::AlignVCenter | Qt::AlignLeft, tr("ØGamePoints"));

}

double ReContraGeneralStatsWidget::contraGamePointAverage() const
{
    return m_contraGamePointAverage;
}

void ReContraGeneralStatsWidget::setContraGamePointAverage(double contraGamePointAverage)
{
    m_contraGamePointAverage = contraGamePointAverage;
}

double ReContraGeneralStatsWidget::reGamePointAverage() const
{
    return m_reGamePointAverage;
}

void ReContraGeneralStatsWidget::setReGamePointAverage(double reGamePointAverage)
{
    m_reGamePointAverage = reGamePointAverage;
}

int ReContraGeneralStatsWidget::contraRoundWins() const
{
    return m_contraRoundWins;
}

void ReContraGeneralStatsWidget::setContraRoundWins(int contraRoundWins)
{
    m_contraRoundWins = contraRoundWins;
}

int ReContraGeneralStatsWidget::reRoundWins() const
{
    return m_reRoundWins;
}

void ReContraGeneralStatsWidget::setReRoundWins(int reRoundWins)
{
    m_reRoundWins = reRoundWins;
}
