#ifndef PLAYERSDELEGATE_H
#define PLAYERSDELEGATE_H

#include "noisygradientitemdelegate.h"

class Player;

class PlayersDelegate : public NoisyGradientItemDelegate
{
public:
    explicit PlayersDelegate(QAbstractItemView *view, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<Player> forIndex(const QModelIndex &index) const;
};

#endif // PLAYERSDELEGATE_H
