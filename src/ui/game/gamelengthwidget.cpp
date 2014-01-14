#include "gamelengthwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include <data/round.h>

GameLengthWidget::GameLengthWidget(QWidget *parent) :
    QWidget(parent)
{
}

void GameLengthWidget::setGame(QSharedPointer<Game> _game)
{
    m_game = _game;

    connect(m_game.data(), SIGNAL(newRoundStarted()), this, SLOT(update()));
}

void GameLengthWidget::update()
{
    this->repaint();
}

void GameLengthWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    int totalHeight = event->rect().height();
    int totalWidth = event->rect().width() - 2;

    int rectHeight = totalHeight * 0.7;
    int heightOffset = totalHeight - rectHeight;

    int rectWidth = totalWidth - 10;
    int widthOffset = totalWidth - rectWidth;

    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setBrush(Qt::white);
    painter.setPen(pen);
    painter.setBrush(Qt::transparent);

    painter.drawRect(event->rect().topLeft().x() + widthOffset/2, event->rect().topLeft().y() + heightOffset/2, rectWidth, rectHeight);

    painter.setPen(Qt::transparent);
    painter.setBrush(QColor("orange"));

    int currentLength = QTime(0,0,0).secsTo(m_game->finishedRoundsLength());
    int totalLength = currentLength + QTime(0,0,0).secsTo(m_game->predictedTimeToPlay());

    int lengthRectWidth = rectWidth * currentLength / totalLength;
    int startWidth = event->rect().topLeft().x() + widthOffset/2 + 1;

    painter.drawRect(startWidth, event->rect().topLeft().y() + heightOffset/2 + 1, lengthRectWidth, rectHeight - 1);

    double currentWidth = startWidth;
    painter.setPen(Qt::white);
    foreach(QSharedPointer<Round> round, m_game->rounds()) {
        if(round->state() == Round::Finished) {

            int relLength = QTime(0,0,0).secsTo(round->length());
            currentWidth += (double)((double)relLength / (double)currentLength) * (double)lengthRectWidth;

            painter.drawLine(currentWidth, event->rect().topLeft().y(), currentWidth, event->rect().bottomLeft().y());
        }
    }

    painter.end();
}

QSize GameLengthWidget::sizeHint() const
{
    QSize size = QWidget::sizeHint();
    size.setWidth(500);
    size.setHeight(20);
    return size;
}
