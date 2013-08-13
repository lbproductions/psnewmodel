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

    painter->setPen(QPen(QColor(0,0,0,0)));
    painter->setBrush(palette.color(QPalette::Base));
    painter->drawRect(rect);

    painter->setPen(QPen(palette.highlight().color()));
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    if(logicalIndex == 0)
        painter->drawLine(rect.topLeft(), rect.topRight());

    painter->drawLine(rect.topRight(), rect.bottomRight());

    painter->setPen(palette.color(QPalette::Text));
    QString text = model()->headerData(logicalIndex, orientation()).toString();
    if(m_model) {
        if(m_model->game()) {
            QSharedPointer<Game> game = m_model->game();
            if(game->currentCardMixer()->name() == text) {
                painter->setPen(Qt::red);
            }
        }
    }
    QTextOption option;
    option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QRect r = rect.adjusted(32,0,0,0);
    painter->drawText(r, text, option);

    QColor color = model()->headerData(logicalIndex, orientation(), Qt::DecorationRole).value<QColor>();
    if(color.isValid()) {
        painter->setPen(QPen(palette.highlight().color()));
        painter->setBrush(color);
        painter->drawRect(QRect(rect.topLeft() + QPoint(9, 6),
                                rect.topLeft() + QPoint(9, 6) + QPoint(16,16)));
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
