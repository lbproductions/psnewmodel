#include "overviewhorizontalheaderview.h"

#include <QTextOption>
#include <QPainter>

OverviewHorizontalHeaderView::OverviewHorizontalHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent)
{
    setSectionResizeMode(QHeaderView::Fixed);
    setDefaultSectionSize(40);
}

QSize OverviewHorizontalHeaderView::sizeHint() const
{
    QSize size = QHeaderView::sizeHint();
    size.setHeight(30);
    return size;
}


void OverviewHorizontalHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();

    QPalette palette = parentWidget()->palette();

    painter->setPen(QPen(Qt::transparent));
    painter->setBrush(palette.color(QPalette::Base));
    painter->drawRect(rect);
    painter->setFont(model()->headerData(logicalIndex, orientation(), Qt::FontRole).value<QFont>());

    QString text = model()->headerData(logicalIndex, orientation()).toString();
    if(!text.isEmpty()) {
        painter->setPen(QPen(palette.highlight().color()));
        painter->drawLine(rect.topLeft() - QPoint(1,0), rect.bottomLeft() - QPoint(1,0));

        painter->setPen(palette.color(QPalette::Text));
        QTextOption option;
        option.setAlignment(Qt::AlignCenter);
        painter->drawText(rect, text, option);
    }

    painter->restore();
}
