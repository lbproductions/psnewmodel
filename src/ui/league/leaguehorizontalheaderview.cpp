#include "leaguehorizontalheaderview.h"

#include <QPainter>

LeagueHorizontalHeaderView::LeagueHorizontalHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent)
{
    setSectionResizeMode(QHeaderView::ResizeToContents);
    setDefaultSectionSize(70);
}

void LeagueHorizontalHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();

    QPalette palette = parentWidget()->palette();

    painter->setPen(QPen(QColor(0,0,0,0)));
    painter->setBrush(palette.color(QPalette::Base));
    painter->drawRect(rect);

    QString text = model()->headerData(logicalIndex, orientation()).toString();
    if(!text.isEmpty()) {
        painter->setPen(QPen(palette.highlight().color()));
        painter->drawLine(rect.topLeft() - QPoint(1,0), rect.bottomLeft() - QPoint(1,0));
        painter->drawLine(rect.topRight(), rect.topRight());

        painter->setPen(palette.color(QPalette::Text));
        QTextOption option;
        option.setAlignment(Qt::AlignCenter);
        painter->drawText(rect, text, option);
    }

    painter->restore();
}
