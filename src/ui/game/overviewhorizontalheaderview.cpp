#include "overviewhorizontalheaderview.h"

#include <data/game.h>
#include <model/gameoverviewmodel.h>

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

    QSharedPointer<Game> game = static_cast<GameOverviewModel *>(model())->game();
    if(!game)
        return;

    painter->save();


    QPalette palette = parentWidget()->palette();

    painter->setPen(QPen(Qt::transparent));
    painter->setBrush(palette.color(QPalette::Base));
    painter->drawRect(rect);
    painter->setFont(model()->headerData(logicalIndex, orientation(), Qt::FontRole).value<QFont>());

    QString text = model()->headerData(logicalIndex, orientation()).toString();

    QTextOption option;
    option.setWrapMode(QTextOption::NoWrap);
    Qt::AlignmentFlag a = static_cast<Qt::AlignmentFlag>(model()->headerData(logicalIndex, orientation(),
                                              GameOverviewModel::AlignmentRole).value<int>());
    option.setAlignment(a | Qt::AlignVCenter);
    painter->setPen(palette.color(QPalette::Text));
    painter->drawText(rect, text, option);

    if(logicalIndex == game->totalRoundCount()) {
        QPen pen = painter->pen();
        pen.setWidth(2);
        pen.setBrush(Qt::white);
        painter->setPen(pen);
        QRect r2 = rect.adjusted(0,1,1,1);
        painter->drawLine(r2.topLeft(), r2.bottomLeft());
    }
    else if(game->players().size() > 0 && logicalIndex % game->players().size() == 0) {
        painter->setPen(QPen(palette.highlight().color()));
        painter->drawLine(rect.topLeft() - QPoint(1,0), rect.bottomLeft() - QPoint(1,0));
    }

    painter->restore();
}
