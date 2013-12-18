#include "recontrapiewidget.h"

#include <QPaintEvent>
#include <limits>
#include <QPainter>

#include <data/game.h>
#include <data/round.h>

ReContraPieWidget::ReContraPieWidget(QWidget *parent) :
    QWidget(parent),
    m_rePerc(0),
    m_contraPerc(0)
{
}

QSize ReContraPieWidget::sizeHint() const
{
    QSize size = QWidget::sizeHint();
    size.setWidth(200);
    size.setHeight(200);
    return size;
}

void ReContraPieWidget::setGames(QList<QSharedPointer<Game> > games)
{
    m_games = games;

    int reCount = 0;
    int contraCount = 0;
    foreach(QSharedPointer<Game> game, m_games) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(round->winnerParty() == Round::Re) {
                reCount++;
            }
            if(round->winnerParty() == Round::Contra) {
                contraCount++;
            }
        }
    }

    m_rePerc = (double)reCount / (double)(reCount + contraCount);
    m_contraPerc = (double)contraCount / (double)(reCount + contraCount);
}

void ReContraPieWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    double scale = 0.1;

    QRect rect = event->rect().adjusted(event->rect().width()*scale,event->rect().height()*scale,-event->rect().width()*scale,-event->rect().height()*scale);

    QPainter painter(this);

    painter.setBrush(QColor(51,51,51));
    painter.drawRoundedRect(event->rect().adjusted(5,5,-5,-5), 10, 10);

    painter.setBrush(Qt::red);

    painter.drawPie(rect, 0, 360*16*m_rePerc);

    painter.setBrush(Qt::black);
    painter.drawPie(rect, 360*16*m_rePerc, 360*16*m_contraPerc);

    QRect restRect = event->rect().adjusted(0,rect.height(),0,0);
    painter.setPen(QColor(228,228,228));
    painter.drawText(restRect, "Re " + QString::number(m_rePerc*100) + " : " + QString::number(m_contraPerc*100) + " Contra", QTextOption(Qt::AlignCenter));
}
