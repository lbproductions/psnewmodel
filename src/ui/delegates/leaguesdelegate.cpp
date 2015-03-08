#include "leaguesdelegate.h"

#include <data/player.h>
#include <ui/model/leaguelistmodel.h>

#include <QPainter>
#include <QAbstractItemView>

LeaguesDelegate::LeaguesDelegate(QAbstractItemView *view, QObject *parent):
    NoisyGradientItemDelegate(view, parent)
{
}

void LeaguesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    QSharedPointer<League> league = forIndex(index);
    if(!league)
        return;

    painter->save();
    drawTitleText(painter, option,
                  league->name(),
                  QPoint(7,7)).topRight();

    QPoint colorPos = option.rect.bottomLeft() + QPoint(7, -23);
    foreach(QSharedPointer<Player> player, league->players()) {
        painter->setBrush(player->color());

        if(player->color() == QColor(Qt::white))
            painter->setPen(Qt::gray);
        else
            painter->setPen(player->color());

        painter->drawRect(QRect(colorPos, QSize(16,16)));
        colorPos += QPoint(18,0);
    }

    painter->setPen(Qt::gray);
    if(option.state & QStyle::State_Selected)
        painter->setPen(Qt::white);

    QRect rect = option.rect.adjusted(7,7,-7,-7);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawText(rect, Qt::AlignRight | Qt::AlignTop,
                      tr("%1 - %2")
                      .arg(league->startDate().toString(Qt::SystemLocaleDate))
                      .arg(league->endDate().toString(Qt::SystemLocaleDate)));

    /*
    rect.adjust(0,0,-13,0);
    QRect siteRect;
    painter->drawText(rect, Qt::AlignRight | Qt::AlignBottom,
                      tr("%1").arg(game->site()->displayString()),
                      &siteRect);

    auto list = game->site()->players();
    if(!list.isEmpty()) {
        painter->setBrush(list.first()->color());
        if(list.first()->color() == QColor(Qt::white))
            painter->setPen(Qt::gray);
        else
            painter->setPen(list.first()->color());
        painter->drawEllipse(QRect(siteRect.topRight() + QPoint(4,3), QSize(10,10)));
    }
    */

    painter->restore();
}

QSize LeaguesDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = NoisyGradientItemDelegate::sizeHint(option, index);
    size.setHeight(size.height() + 40);
    return size;
}

QSharedPointer<League> LeaguesDelegate::forIndex(const QModelIndex &index) const
{
    return static_cast<LeagueSortFilterModel *>(view()->model())->objectByIndex(index);
}
