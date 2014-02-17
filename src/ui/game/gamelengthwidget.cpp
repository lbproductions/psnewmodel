#include "gamelengthwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include <data/round.h>

GameLengthWidget::GameLengthWidget(QWidget *parent) :
    QWidget(parent),
    m_context(TotalTime)
{
    this->setFixedWidth(80);
    setCursor(Qt::PointingHandCursor);
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

void GameLengthWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_context == Duration) {
        m_context = TotalTime;
    }
    else if(m_context == TotalTime) {
        m_context = Duration;
    }

    update();
}

void GameLengthWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if(!m_game)
        return;

    QPainter painter(this);

    int scalaWidth = 50;

    int totalHeight = event->rect().height();
    int totalWidth = event->rect().width() - scalaWidth;

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

    if(m_context == TotalTime && !m_game->rounds().isEmpty()) {
        painter.drawText(QRect(event->rect().bottomRight().x()-scalaWidth, currentHeight - 7, scalaWidth, 15), Qt::AlignRight,
                         m_game->rounds().first()->startTime().time().toString("hh:mm"));
    }
    else if(m_context == Duration) {
        painter.drawText(QRect(event->rect().bottomRight().x()-scalaWidth, currentHeight - 7, scalaWidth, 15), Qt::AlignRight,
                         QTime(0,0,0).toString("hh:mm") + "h");
    }

    int count = 0;
    bool averagePainted = false;
    foreach(QSharedPointer<Round> round, m_game->rounds()) {
        if(round->state() == Round::Finished) {

            double relLength = QTime(0,0,0).secsTo(round->length());
            currentHeight -= (double)((double)relLength / (double)currentLength) * (double)lengthRectHeight;

            painter.drawLine(event->rect().topLeft().x(), currentHeight, event->rect().topRight().x() - scalaWidth, currentHeight);

            if(m_game->finishedRoundCount()/2 == count && count > 1 && !averagePainted && m_context == Duration) {
                painter.drawText(QRect(event->rect().bottomRight().x()-scalaWidth, currentHeight - 7, scalaWidth, 15), Qt::AlignRight,
                                 "\u00D8" + m_game->averageRoundLength().toString("mm:ss"));

                averagePainted = true;
            }

            count++;
        }
    }

    if(m_context == TotalTime) {
        painter.drawText(QRect(event->rect().bottomRight().x()-scalaWidth, currentHeight - 7, scalaWidth, 15), Qt::AlignRight,
                         QTime::currentTime().toString("hh:mm"));
    }
    else if(m_context == Duration) {
        painter.drawText(QRect(event->rect().bottomRight().x()-scalaWidth, currentHeight - 7, scalaWidth, 15), Qt::AlignRight,
                         m_game->length().toString("hh:mm") + "h");
    }


    if(m_game->state() != Game::Finished) {
        if(m_context == TotalTime) {
            painter.drawText(QRect(event->rect().bottomRight().x()-scalaWidth, event->rect().topLeft().y() + heightOffset * 0.25 - 7, scalaWidth, 15),
                             Qt::AlignRight,
                             m_game->predictedEndTime().toString("hh:mm"));
        }
        else if(m_context == Duration) {
            painter.drawText(QRect(event->rect().bottomRight().x()-scalaWidth, event->rect().topLeft().y() + heightOffset * 0.25 - 7, scalaWidth, 15),
                             Qt::AlignRight,
                             m_game->predictedGameLength().toString("hh:mm") + "h");
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
