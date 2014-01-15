#include "gamelengthwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include <data/round.h>

GameLengthWidget::GameLengthWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedWidth(35);
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

    if(!m_game)
        return;

    QPainter painter(this);

    int totalHeight = event->rect().height();
    int totalWidth = event->rect().width();

    int rectHeight = totalHeight - 30;
    int heightOffset = totalHeight - rectHeight;

    double rectWidth = totalWidth * 0.7;
    double widthOffset = totalWidth - rectWidth;

    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setBrush(Qt::white);
    painter.setPen(pen);
    painter.setBrush(Qt::transparent);

    painter.drawRect(event->rect().topLeft().x() + widthOffset/2, event->rect().topLeft().y() + heightOffset * 0.25, rectWidth, rectHeight);

    painter.setPen(Qt::transparent);
    painter.setBrush(QColor("orange"));

    double currentLength = QTime(0,0,0).secsTo(m_game->finishedRoundsLength());
    double totalLength = currentLength + QTime(0,0,0).secsTo(m_game->predictedTimeToPlay());

    double lengthRectHeight = rectHeight * currentLength / totalLength;
    double startHeight = event->rect().bottomLeft().y() - lengthRectHeight - heightOffset * 0.75 + 1;

    painter.drawRect(event->rect().bottomLeft().x() + widthOffset/2 + 1, startHeight, rectWidth - 1, lengthRectHeight);

    double currentHeight = event->rect().bottomLeft().y() - heightOffset * 0.75 + 1;
    painter.setPen(Qt::white);
    foreach(QSharedPointer<Round> round, m_game->rounds()) {
        if(round->state() == Round::Finished) {

            double relLength = QTime(0,0,0).secsTo(round->length());
            currentHeight -= (double)((double)relLength / (double)currentLength) * (double)lengthRectHeight;

            painter.drawLine(event->rect().topLeft().x(), currentHeight, event->rect().topRight().x(), currentHeight);
        }
    }

    QFont font = painter.font();
    font.setPixelSize(10);
    painter.drawText(QRect(event->rect().bottomLeft().x(), event->rect().bottomLeft().y()-15, totalWidth, 15),
                      Qt::AlignCenter, QString::number((int)m_game->completedPercentage()) + "%");

    painter.end();
}

QSize GameLengthWidget::sizeHint() const
{
    QSize size = QWidget::sizeHint();
    size.setWidth(20);
    size.setHeight(500);
    return size;
}
