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

    QColor color = index.data(Qt::BackgroundColorRole).value<QColor>();
    if(color.isValid()) {
        painter->setBrush(color);
        painter->drawRect(r);
    }

    QString text = index.data().toString();
    if(!text.isEmpty()) {
        painter->setPen(palette.color(QPalette::Text));
        QTextOption to;
        to.setAlignment(Qt::AlignCenter);
        painter->drawText(option.rect, text, to);
    }

    color = index.data(GameOverviewModel::MitspielerColorRole).value<QColor>();
    if(color.isValid()) {
        painter->setBrush(color);
        painter->drawRect(QRect(option.rect.topLeft() + QPoint(16,8), option.rect.topLeft() + QPoint(32,24)));
    }

    color = index.data(Qt::DecorationRole).value<QColor>();
    if(color.isValid()) {
        painter->setBrush(color);
        painter->drawRect(QRect(option.rect.topLeft() + QPoint(4,3), option.rect.topLeft() + QPoint(20,19)));
    }

    painter->restore();
}

GameOverviewModel *OverviewDelegate::model() const
{
    return m_model;
}

void OverviewDelegate::setGameModel(GameOverviewModel *model)
{
    m_model = model;
}
