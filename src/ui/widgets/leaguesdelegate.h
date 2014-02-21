#ifndef LEAGUESDELEGATE_H
#define LEAGUESDELEGATE_H

#include "noisygradientitemdelegate.h"
#include <data/league.h>

class LeaguesDelegate  : public NoisyGradientItemDelegate
{
public:
    LeaguesDelegate(QAbstractItemView *view, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<League> forIndex(const QModelIndex &index) const;
};

#endif // LEAGUESDELEGATE_H
