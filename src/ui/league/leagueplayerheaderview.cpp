#include "leagueplayerheaderview.h"

#include <QPainter>

#include <ui/model/leagueclassementmodel.h>

LeaguePlayerHeaderView::LeaguePlayerHeaderView(Qt::Orientation orientation, QWidget *parent):
    QHeaderView(orientation, parent),
    m_model(0)
{
    //setSectionResizeMode(QHeaderView::Fixed);
}

QSize LeaguePlayerHeaderView::sizeHint() const
{
    QSize size = QHeaderView::sizeHint();
    size += QSize(25,0);
    size.setHeight(30);
    return size;
}

QSize LeaguePlayerHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    QSize size = QHeaderView::sectionSizeFromContents(logicalIndex);
    size += QSize(25,0);
    size.setHeight(30);
    return size;
}

void LeaguePlayerHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();

    QPalette palette = parentWidget()->palette();

    // background
    QPen pen = painter->pen();
    pen.setColor(QColor(0,0,0,0));
    painter->setPen(pen);
    painter->setBrush(palette.color(QPalette::Base));
    painter->drawRect(rect);

    pen.setColor(palette.highlight().color());
    painter->setPen(pen);
    painter->drawLine(rect.topRight(), rect.bottomRight());
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    if(logicalIndex == 0)
        painter->drawLine(rect.topLeft(), rect.topRight());

    QString text = model()->headerData(logicalIndex, orientation()).toString();
    painter->setPen(palette.color(QPalette::Text));
    QTextOption option;
    option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QRect r = rect.adjusted(32,0,0,0);
    painter->drawText(r, text, option);

    // player color
    QColor color = model()->headerData(logicalIndex, orientation(), Qt::DecorationRole).value<QColor>();
    if(color.isValid()) {
        painter->setPen(QPen(palette.highlight().color()));
        painter->setBrush(color);
        painter->drawRect(QRect(rect.topLeft() + QPoint(9, 6),
                                rect.topLeft() + QPoint(9, 6) + QPoint(16,16)));
    }

    painter->restore();
}

LeagueClassementModel *LeaguePlayerHeaderView::model() const
{
    return m_model;
}

void LeaguePlayerHeaderView::setLeagueClassementModel(LeagueClassementModel *model)
{
    m_model = model;
}
