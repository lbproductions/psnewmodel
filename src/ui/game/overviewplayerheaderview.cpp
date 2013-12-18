#include "overviewplayerheaderview.h"

#include <model/gameinformationmodel.h>
#include <data/game.h>
#include <data/player.h>

#include <QPainter>

OverviewPlayerHeaderView::OverviewPlayerHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent),
    m_model(0)
{
    setSectionResizeMode(QHeaderView::Fixed);
}

QSize OverviewPlayerHeaderView::sizeHint() const
{
    QSize size = QHeaderView::sizeHint();
    size += QSize(45,0);
    size.setHeight(30);
    return size;
}

QSize OverviewPlayerHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    QSize size = QHeaderView::sectionSizeFromContents(logicalIndex);
    size += QSize(45,0);
    size.setHeight(30);
    return size;
}

void OverviewPlayerHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();

    QPalette palette = parentWidget()->palette();

    bool cardMixerMode = false;
    bool hasPflichtSolo = false;
    bool border = false;

    QString text = model()->headerData(logicalIndex, orientation()).toString();
    QColor color = model()->headerData(logicalIndex, orientation(), Qt::DecorationRole).value<QColor>();

    if(m_model) {
        if(m_model->game()) {
            QSharedPointer<Game> game = m_model->game();
            if(game->state() != Game::Finished && game->currentCardMixer() && game->currentCardMixer()->name() == text) {
                cardMixerMode = true;
            }
            foreach(QSharedPointer<Player> player, game->players()) {
                if(player->name() == text) {
                    hasPflichtSolo = game->hasPflichtSolo(player);
                }
            }
            if(game->players().size() == logicalIndex+1)
                border = true;
        }
    }

    // background
    QPen pen = painter->pen();
    pen.setColor(QColor(0,0,0,0));
    painter->setPen(pen);
    painter->setBrush(palette.color(QPalette::Base));
    if(cardMixerMode && color.isValid()) {
        //painter->setBrush(QColor(120,70,0));
        painter->setBrush(color.darker(250));
    }
    painter->drawRect(rect);

    pen.setColor(palette.highlight().color());
    painter->setPen(pen);
    painter->drawLine(rect.topRight(), rect.bottomRight());
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    if(logicalIndex == 0)
        painter->drawLine(rect.topLeft(), rect.topRight());
    if(border) {
        pen.setColor(Qt::white);
        //pen.setWidth(0);
        painter->setPen(pen);
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());
        //pen.setWidth(0);
        pen.setColor(palette.highlight().color());
        painter->setPen(pen);
    }

    // player name
    painter->setPen(palette.color(QPalette::Text));
    QTextOption option;
    option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QRect r = rect.adjusted(32,0,0,0);
    painter->drawText(r, text, option);

    // player color
    if(color.isValid()) {
        painter->setPen(QPen(palette.highlight().color()));
        painter->setBrush(color);
        painter->drawRect(QRect(rect.topLeft() + QPoint(9, 6),
                                rect.topLeft() + QPoint(9, 6) + QPoint(16,16)));
    }


    if(cardMixerMode && hasPflichtSolo) {
        QRect cRect = rect.adjusted(0,0,-5,0);
        static QPixmap pixmap = QPixmap(":/sidebar/games_green.png").scaledToHeight(cRect.height()-10);
        int offset = rect.height() - pixmap.height();
        painter->drawPixmap(rect.topRight().x()-pixmap.width()-5, cRect.topRight().y()+offset/2, pixmap);
    }
    else if(cardMixerMode) {
        QRect cRect = rect.adjusted(0,0,-5,0);
        static QPixmap pixmap = QPixmap(":/sidebar/games.png").scaledToHeight(cRect.height()-10);
        int offset = rect.height() - pixmap.height();
        painter->drawPixmap(rect.topRight().x()-pixmap.width()-5, cRect.topRight().y()+offset/2, pixmap);
    }
    else if(hasPflichtSolo) {
        QRect cRect = rect.adjusted(0,0,-15,0);
        painter->setBrush(Qt::green);
        painter->drawEllipse(QPoint(cRect.topRight().x(), cRect.topRight().y()+cRect.height()/2), 7, 7);
    }

    painter->restore();
}

GameInformationModel *OverviewPlayerHeaderView::model() const
{
    return m_model;
}

void OverviewPlayerHeaderView::setGameModel(GameInformationModel *model)
{
    m_model = model;
}
