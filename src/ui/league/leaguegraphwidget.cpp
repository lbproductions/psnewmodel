#include "leaguegraphwidget.h"

#include <data/league.h>
#include <data/matchday.h>
#include <data/player.h>
#include <data/game.h>

#include <QPaintEvent>
#include <limits>
#include <QPainter>

LeagueGraphWidget::LeagueGraphWidget(QWidget *parent) :
    GraphWidget(parent),
    m_type(PlacementGraph)
{
}

void LeagueGraphWidget::setLeague(QSharedPointer<League> league)
{
    m_league = league;

    m_matchdays.clear();
    m_colors.clear();
    m_maxY = std::numeric_limits<int>::min();
    m_minY = std::numeric_limits<int>::max();

    QList<QSharedPointer<Matchday> > matchdays = m_league->matchdays();
    QList<QSharedPointer<Player> > players = m_league->players();

    for(int i = 0; i<League::currentMatchDayNumber; i++) {
        QSharedPointer<Matchday> m = matchdays.at(i);
        QList<int> matchday;
        QList<QColor> color;
        foreach(QSharedPointer<Player> p, players) {
            int y;
            if(m_type == PlacementGraph) {
                y = players.size() + 1 - m->placementOfPlayer(p);
                setMaxY(qMax(y, maxY()));
                setMinY(qMin(y, minY()));
                matchday.append(y);
            }
            else if(m_type == AverageGraph) {
                y = m->average(p);
                setMaxY(qMax(y, maxY()));
                setMinY(qMin(y, minY()));
                matchday.append(y);
            }
            else if(m_type == PointsGraph) {
                if(m->game()->players().contains(p)) {
                    y = m->game()->totalPoints(p);
                    setMaxY(qMax(y, maxY()));
                    setMinY(qMin(y, minY()));
                    matchday.append(y);
                    color.append(p->color());
                }
            }
            else if(m_type == TotalPointsGraph) {

            }

        }
        m_matchdays.append(matchday);
        m_colors.append(color);
    }

    if(m_type == PlacementGraph) {
        m_maxY += 1;
        m_minY -= 2;
    }
    else if(m_type == AverageGraph || m_type == PointsGraph) {
        m_maxY += 5;
        m_minY -= 5;
    }


    update();
}

QSize LeagueGraphWidget::sizeHint() const
{
    QSize size = QWidget::sizeHint();
    if(m_league)
        size.setWidth((League::currentMatchDayNumber + 1) * m_cellWidth + originX());
    return size;
}

int LeagueGraphWidget::translateX(int x) const
{
    return x * m_cellWidth - 1;
}

LeagueGraphWidget::GraphType LeagueGraphWidget::type()
{
    return m_type;
}

void LeagueGraphWidget::setType(LeagueGraphWidget::GraphType type)
{
    m_type = type;

    updateGraphs();
}

void LeagueGraphWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    if(!m_league || m_league->matchdays().isEmpty()) {
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
    int nextInterval = ((y/10)-1)*10;
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

    if(!m_league)
        return;

    if(m_matchdays.isEmpty())
        return;

    QList<QSharedPointer<Player> > players = m_league->players();
    int playerCount = players.size();
    if(playerCount == 0)
        return;

    for(int x = 0; translateX(x) < width(); ++x) {

        pen.setStyle(Qt::SolidLine);
        pen.setColor(palette().color(QPalette::Highlight));
        if(m_type == PointsGraph) {
            pen.setWidth(2);
        }

        painter.setPen(pen);
        painter.drawLine(QPointF(translateX(x), 0), QPointF(translateX(x), height()));
    }

    painter.setRenderHint(QPainter::Antialiasing);

    QFont f = painter.font();
    f.setPixelSize(18);
    painter.setFont(f);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(3);


    QList<int> previousMatchday;
    int matchdayNumber = 0;
    foreach(QList<int> currentMatchday, m_matchdays) {
        for(int playerIndex = 0; playerIndex < currentMatchday.size(); ++playerIndex) {
            int y = currentMatchday.at(playerIndex);

            if(m_type != PointsGraph) {
                pen.setColor(players.at(playerIndex)->color());
                painter.setPen(pen);

                int previousY = 0;
                if(!previousMatchday.isEmpty())
                    previousY = previousMatchday.at(playerIndex);

                pen.setStyle(Qt::SolidLine);

                for(int otherPlayerIndex = 0; otherPlayerIndex < playerIndex; ++otherPlayerIndex) {
                    int otherY = currentMatchday.at(otherPlayerIndex);
                    int otherPreviousY = 0;
                    if(!previousMatchday.isEmpty())
                        otherPreviousY = previousMatchday.at(otherPlayerIndex);

                    if(otherY == y && otherPreviousY == previousY) {
                        pen.setStyle(Qt::DotLine);
                    }
                }
                painter.setPen(pen);

                painter.drawLine(QPoint(translateX(matchdayNumber), translateY(previousY)),
                                 QPoint(translateX(matchdayNumber + 1), translateY(y)));
            }
            else {
                pen.setStyle(Qt::SolidLine);
                pen.setWidth(0);
                pen.setColor(Qt::black);
                painter.setPen(pen);

                QList<QColor> colors = m_colors.at(matchdayNumber);
                painter.setBrush(colors.at(playerIndex));

                QPolygonF polygon;
                polygon << QPoint(translateX(matchdayNumber)+playerIndex*cellWidth()/currentMatchday.size(), originY())
                        << QPoint(translateX(matchdayNumber)+playerIndex*cellWidth()/currentMatchday.size(), translateY(y))
                        << QPoint(translateX(matchdayNumber)+(playerIndex+1)*cellWidth()/currentMatchday.size(), translateY(y))
                        << QPoint(translateX(matchdayNumber)+(playerIndex+1)*cellWidth()/currentMatchday.size(), originY());

                painter.drawPolygon(polygon);
            }
        }
        previousMatchday = currentMatchday;
        ++matchdayNumber;
    }

    QList<int> lastMatchday = m_matchdays.last();
    for(int playerIndex = 0; playerIndex < lastMatchday.size(); ++playerIndex) {
        pen.setColor(Qt::white);
        pen.setWidth(2);
        painter.setPen(pen);
        int points = lastMatchday.at(playerIndex);
        int yPos = translateY(points) + 5;
        for(int otherPlayerIndex = 0; otherPlayerIndex < lastMatchday.size(); ++otherPlayerIndex) {
            if(otherPlayerIndex == playerIndex)
                continue;

            int otherPoints = lastMatchday.at(otherPlayerIndex);
            int diff = points - otherPoints;
            if(diff != 0 && qAbs(diff) <= 3) {
                if(points < otherPoints) {
                    yPos += 5;
                }
                else {
                    yPos -= 5;
                }
            }
        }

        if(m_type == PlacementGraph) {
            painter.drawText(QPoint(translateX(m_matchdays.size()) + 5,
                                    yPos),
                             QString::number(players.size()+1-points));
        }
        else if(m_type == AverageGraph) {
            painter.drawText(QPoint(translateX(m_matchdays.size()) + 5,
                                    yPos),
                             QString::number(points));
        }
    }
}

void LeagueGraphWidget::updateGraphs()
{
    if(!m_league)
        return;
    setLeague(m_league);

    emit graphUpdated();
}
