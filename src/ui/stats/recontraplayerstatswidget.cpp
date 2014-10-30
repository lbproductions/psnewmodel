#include "recontraplayerstatswidget.h"

#include <QPainter>
#include <QDebug>

#include <data/player.h>

ReContraPlayerStatsWidget::ReContraPlayerStatsWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumHeight(100);
}

void ReContraPlayerStatsWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(!m_player) {
        return;
    }

    // Draw name
    painter.setBrush(m_player->color());
    painter.drawRect(0, 0, 20, 20);
    painter.drawText(QRect(30, 0, this->width()/3, 20), Qt::AlignVCenter, m_player->name());
    //painter.drawText(30,0,this->width()/3, 20, Qt::AlignLeft, m_player->name());

    int barHeight = 10;
    int startHeight = 30;

    painter.save();
    int midX = this->width()/2;
    int x = valueToCoordinates(m_rePercentage);



    painter.setPen(Qt::transparent);
    painter.setBrush(percentageColor(m_rePercentage));
    if(m_rePercentage > 50) {
        painter.drawRect(midX, startHeight, x-midX, barHeight);
        painter.setPen(Qt::white);
        painter.drawText(QRect(x+10, startHeight, 40, barHeight), Qt::AlignVCenter | Qt::AlignLeft, QString::number(m_rePercentage)+"%");
        painter.setPen(Qt::transparent);
    }
    else {
        painter.drawRect(x, startHeight, midX-x, barHeight);
        painter.setPen(Qt::white);
        painter.drawText(QRect(x-50, startHeight, 40, barHeight), Qt::AlignVCenter | Qt::AlignRight, QString::number(m_rePercentage)+"%");
        painter.setPen(Qt::transparent);
    }
    startHeight += 2* barHeight;

    x = valueToCoordinates(m_reWinPercentage);
    painter.setBrush(percentageColor(m_reWinPercentage));
    if(m_reWinPercentage > 50) {
        painter.drawRect(midX, startHeight, x-midX, barHeight);
        painter.setPen(Qt::white);
        painter.drawText(QRect(x+10, startHeight, 40, barHeight), Qt::AlignVCenter | Qt::AlignLeft, QString::number(m_reWinPercentage)+"%");
        painter.setPen(Qt::transparent);
    }
    else {
        painter.drawRect(x, startHeight, midX-x, barHeight);
        painter.drawRect(x, startHeight, midX-x, barHeight);
        painter.setPen(Qt::white);
        painter.drawText(QRect(x-50, startHeight, 40, barHeight), Qt::AlignVCenter | Qt::AlignRight, QString::number(m_reWinPercentage)+"%");
        painter.setPen(Qt::transparent);
    }
    startHeight += 2* barHeight;

    x = valueToCoordinates(m_contraWinPercentage);
    painter.setBrush(percentageColor(m_contraWinPercentage));
    if(m_contraWinPercentage > 50) {
        painter.drawRect(midX, startHeight, x-midX, barHeight);
        painter.setPen(Qt::white);
        painter.drawText(QRect(x+10, startHeight, 40, barHeight), Qt::AlignVCenter | Qt::AlignLeft, QString::number(m_contraWinPercentage)+"%");
        painter.setPen(Qt::transparent);
    }
    else {
        painter.drawRect(x, startHeight, midX-x, barHeight);
        painter.drawRect(x, startHeight, midX-x, barHeight);
        painter.setPen(Qt::white);
        painter.drawText(QRect(x-50, startHeight, 40, barHeight), Qt::AlignVCenter | Qt::AlignRight, QString::number(m_contraWinPercentage)+"%");
        painter.setPen(Qt::transparent);
    }
    painter.restore();

    painter.save();
    painter.setBrush(Qt::white);
    QPen pen = painter.pen();
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawLine(this->width()/2, 0, this->width()/2, this->height());
    painter.restore();
}

QColor ReContraPlayerStatsWidget::percentageColor(double percentage)
{
    if(percentage < 25) {
        return QColor("red");
    }
    else if(percentage < 50) {
        return QColor(123,0,0);
    }
    else if(percentage < 75) {
        return QColor(0, 123, 0);
    }
    else {
        return QColor(0,200,0);
    }
}

int ReContraPlayerStatsWidget::valueToCoordinates(int value)
{
    int nullX = 50;

    float x2Distance = value;
    float x2Scale = x2Distance / 100;
    return nullX + (this->width()-2*nullX) * x2Scale;
}

QSharedPointer<Player> ReContraPlayerStatsWidget::player() const
{
    return m_player;
}

void ReContraPlayerStatsWidget::setPlayer(const QSharedPointer<Player> &player)
{
    m_player = player;
}

int ReContraPlayerStatsWidget::contraWinPercentage() const
{
    return m_contraWinPercentage;
}

void ReContraPlayerStatsWidget::setContraWinPercentage(int contraWinPercentage)
{
    m_contraWinPercentage = contraWinPercentage;
}

int ReContraPlayerStatsWidget::reWinPercentage() const
{
    return m_reWinPercentage;
}

void ReContraPlayerStatsWidget::setReWinPercentage(int reWinPercentage)
{
    m_reWinPercentage = reWinPercentage;
}

int ReContraPlayerStatsWidget::rePercentage() const
{
    return m_rePercentage;
}

void ReContraPlayerStatsWidget::setRePercentage(int rePercentage)
{
    m_rePercentage = rePercentage;
}

