#include "overviewdelegate.h"

#include <model/gameoverviewmodel.h>

#include <QPainter>
#include <QTextOption>

OverviewDelegate::OverviewDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void OverviewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QPalette palette = static_cast<QWidget *>(this->parent())->palette();

    painter->setPen(QPen(palette.color(QPalette::Highlight)));
    painter->setBrush(palette.color(QPalette::Base));

    QRect r = option.rect.adjusted(-1,0,0,-1);
    if(index.row() != 0)
        r.adjust(0,-1,0,0);


    painter->drawRect(r);

    QString text = index.data().toString();
    if(!text.isEmpty()) {
        if(index.column() > 0) {
            int points = index.data().toInt();
            if(points < 0) {
                QColor color(237, 60, 46);
                for(;points > -10; --points) {
                    color = color.darker(115 - points);
                }
                painter->setBrush(color);
                painter->drawRect(r);
            }
            else if(points > 0) {
                QColor color(100, 211, 10);
                for(;points < 10; ++points) {
                    color = color.darker(115 - points);
                }
                painter->setBrush(color);
                painter->drawRect(r);
            }
        }
        painter->setPen(palette.color(QPalette::Text));
        QTextOption to;
        to.setAlignment(Qt::AlignCenter);
        painter->drawText(option.rect, text, to);
    }

    if(index.row() == GameOverviewModel::HochzeitenRow) {

        QColor color = index.data(GameOverviewModel::HochzeitMitspielerColorRole).value<QColor>();
        if(color.isValid()) {
            painter->setBrush(color);
            painter->drawRect(QRect(option.rect.topLeft() + QPoint(16,8), option.rect.topLeft() + QPoint(32,24)));
        }

        color = index.data(Qt::DecorationRole).value<QColor>();
        if(color.isValid()) {
            painter->setBrush(color);
            painter->drawRect(QRect(option.rect.topLeft() + QPoint(4,3), option.rect.topLeft() + QPoint(20,19)));
        }
    }

    painter->restore();
}
