#include "gamegraphwidget.h"

#include <data/game.h>
#include <data/round.h>
#include <data/player.h>

#include <QPaintEvent>
#include <limits>
#include <QPainter>

GameGraphWidget::GameGraphWidget(QWidget *parent) :
    GraphWidget(parent)
{
}

QSize GameGraphWidget::sizeHint() const
{
    QSize size = QWidget::sizeHint();
    if(m_game)
        size.setWidth((m_game->totalRoundCount() + 1) * columnWidth() + originX());
    return size;
}

QSharedPointer<Game> GameGraphWidget::game() const
{
    return m_game;
}

void GameGraphWidget::setGame(const QSharedPointer<Game> &game)
{
    m_game = game;

    m_rounds.clear();
    m_maxY = std::numeric_limits<int>::min();
    m_minY = std::numeric_limits<int>::max();

    QList<QSharedPointer<Round> > rounds = m_game->rounds();
    QList<QSharedPointer<Player> > players = m_game->players();

    foreach(QSharedPointer<Round> r, rounds) {
        if(r->state() != Round::Finished)
            break;

        QList<int> round;
        foreach(QSharedPointer<Player> p, players) {
            if(r->state() != Round::Finished)
                break;

            int y = r->totalPoints(p);
            setMaxY(qMax(y, maxY()));
            setMinY(qMin(y, minY()));
            round.append(y);
        }
        m_rounds.append(round);
    }

    m_maxY += 5;
    m_minY -= 5;

    update();
}

void GameGraphWidget::paintEvent(QPaintEvent *e)
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

    if(!m_game)
        return;

    if(m_rounds.isEmpty())
        return;

    QList<QSharedPointer<Player> > players = m_game->players();
    int playerCount = players.size();
    if(playerCount == 0)
        return;

    for(int x = 0; translateX(x) < width(); ++x) {
        if(x % playerCount != 0) {
            pen.setStyle(Qt::DotLine);
            pen.setColor(palette().color(QPalette::Highlight).darker(130));
        }
        else {
            pen.setStyle(Qt::SolidLine);
            pen.setColor(palette().color(QPalette::Highlight));
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


    QList<int> previousRound;
    int roundNumber = 0;
    foreach(QList<int> currentRound, m_rounds) {
        for(int playerIndex = 0; playerIndex < currentRound.size(); ++playerIndex) {
            pen.setColor(players.at(playerIndex)->color());
            painter.setPen(pen);

            int y = currentRound.at(playerIndex);
            int previousY = 0;
            if(!previousRound.isEmpty())
                previousY = previousRound.at(playerIndex);

            pen.setStyle(Qt::SolidLine);

            for(int otherPlayerIndex = 0; otherPlayerIndex < playerIndex; ++otherPlayerIndex) {
                int otherY = currentRound.at(otherPlayerIndex);
                int otherPreviousY = 0;
                if(!previousRound.isEmpty())
                    otherPreviousY = previousRound.at(otherPlayerIndex);

                if(otherY == y && otherPreviousY == previousY) {
                    pen.setStyle(Qt::DotLine);
                }
            }
            painter.setPen(pen);

            painter.drawLine(QPoint(translateX(roundNumber), translateY(previousY)),
                             QPoint(translateX(roundNumber + 1), translateY(y)));
        }
        previousRound = currentRound;
        ++roundNumber;
    }

    QList<int> lastRound = m_rounds.last();
    for(int playerIndex = 0; playerIndex < lastRound.size(); ++playerIndex) {
        pen.setColor(Qt::white);
        pen.setWidth(2);
        painter.setPen(pen);
        int points = lastRound.at(playerIndex);
        int yPos = translateY(points) + 5;
        for(int otherPlayerIndex = 0; otherPlayerIndex < lastRound.size(); ++otherPlayerIndex) {
            if(otherPlayerIndex == playerIndex)
                continue;

            int otherPoints = lastRound.at(otherPlayerIndex);
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

        painter.drawText(QPoint(translateX(m_rounds.size()) + 5,
                                yPos),
                         QString::number(points));
    }
}

void GameGraphWidget::updateGraphs()
{
    if(!m_game)
        return;
    setGame(m_game);

    emit updated();
}
