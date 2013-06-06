#include "graphwidget.h"

#include <data/game.h>
#include <data/round.h>
#include <data/player.h>

#include <QPaintEvent>
#include <limits>
#include <QPainter>

GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent),
    m_maxY(std::numeric_limits<int>::min()),
    m_minY(std::numeric_limits<int>::max())
{
}

QSize GraphWidget::sizeHint() const
{
    QSize size = QWidget::sizeHint();
    if(m_game)
        size.setWidth((m_game->totalRoundCount() + 1) * 40 + originX());
    return size;
}

QSharedPointer<Game> GraphWidget::game() const
{
    return m_game;
}

void GraphWidget::setGame(const QSharedPointer<Game> &game)
{
    m_game = game;

    m_values.clear();
    m_maxY = std::numeric_limits<int>::min();
    m_minY = std::numeric_limits<int>::max();

    QList<QSharedPointer<Round> > rounds = m_game->rounds();

    foreach(QSharedPointer<Player> p, m_game->players()) {
        QList<int> values;
        foreach(QSharedPointer<Round> r, rounds) {
            if(r->state() != Round::Finished)
                break;

            int y = r->totalPoints(p);
            setMaxY(qMax(y, maxY()));
            setMinY(qMin(y, minY()));
            values.append(y);
        }
        m_values.append(values);
    }

    m_maxY += 5;
    m_minY -= 5;

    update();
}

int GraphWidget::originX() const
{
    return 0;
}

void GraphWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);

    if(!m_game || m_game->rounds().isEmpty()) {
        m_maxY = 55;
        m_minY = -55;
    }

    painter.setPen(QPen(QColor(0,0,0,0)));
    painter.setBrush(palette().color(QPalette::Base));
    painter.drawRect(QRect(0,0,width(),height()));

    QPen pen;
    pen.setWidth(3);
    pen.setColor(palette().highlightedText().color());
    painter.setPen(pen);
    painter.drawLine(QPointF(originX(), originY()), QPointF(width(), originY()));

    int y = minY();
    int nextInterval = ((y/10)+1)*10;
    int previousInterval = ((y/10)-1)*10;
    if(qAbs(previousInterval - y) <= 5)
        y = nextInterval;
    else
        y = previousInterval;

    for(; y < maxY(); y += 10) {
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
        painter.drawLine(QPointF(originX(), translateY(y)), QPointF(width(), translateY(y)));
    }

    pen.setWidth(1);
    pen.setColor(palette().highlight().color());
    painter.setPen(pen);

    if(!m_game)
        return;

    int playerCount = m_game->players().size();
    if(playerCount == 0)
        return;

    for(int x = playerCount; translateX(x) < width(); x+=playerCount) {
        painter.drawLine(QPointF(translateX(x), 0), QPointF(translateX(x), height()));
    }

    painter.setRenderHint(QPainter::Antialiasing);

    QFont f = painter.font();
    f.setPixelSize(18);
    painter.setFont(f);

    int p = 0;
    foreach(QList<int> graph, m_values) {
        pen.setColor(m_game->players().at(p)->color());
        pen.setWidth(3);
        painter.setPen(pen);
        int previousY = 0;
        int count = graph.size();
        for(int i = 0; i < count; ++i) {
            int y = graph.at(i);
            painter.drawLine(QPoint(translateX(i), translateY(previousY)),
                             QPoint(translateX(i + 1), translateY(y)));
            previousY = y;

            if(i == count - 1) {
                pen.setColor(Qt::white);
                pen.setWidth(2);
                painter.setPen(pen);
                painter.drawText(QPoint(translateX(i + 1) + 5, translateY(y) + 5), QString::number(y));
            }
        }
        ++p;
    }
}

int GraphWidget::originY() const
{
    return qAbs(maxY()) * pixelsPerPoint();
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
