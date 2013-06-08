#include "playerslistdelegate.h"

#include <QPainter>

PlayersListDelegate::PlayersListDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

QSize PlayersListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(30);
    return size;
}

void PlayersListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QPalette palette = option.palette;

    painter->setPen(QPen(QColor(0,0,0,0)));
    if(option.state & QStyle::State_Selected) {
        painter->setBrush(palette.color(QPalette::AlternateBase));
    }
    else {
        painter->setBrush(QColor(55,55,55));
    }
    painter->drawRect(option.rect);

    painter->setPen(QPen(palette.highlight().color()));
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    painter->setPen(palette.color(QPalette::Text));
    QTextOption to;
    to.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QRect r = option.rect.adjusted(32,0,0,0);
    painter->drawText(r, index.data().toString(), to);

    painter->setPen(QPen(palette.highlight().color()));
    painter->drawRect(QRect(option.rect.topLeft() + QPoint(9, 6),
                             option.rect.topLeft() + QPoint(9, 6) + QPoint(16,16)));
    painter->drawPixmap(QRect(option.rect.topLeft() + QPoint(10, 7),
                             option.rect.topLeft() + QPoint(9, 6) + QPoint(16,16)),
                        index.data(Qt::DecorationRole).value<QIcon>().pixmap(QSize(16,16)));
    painter->restore();
}
