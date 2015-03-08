#ifndef GAMESDELEGATE_H
#define GAMESDELEGATE_H

#include "noisygradientitemdelegate.h"

class Game;

class GamesDelegate : public NoisyGradientItemDelegate
{
public:
    explicit GamesDelegate(QAbstractItemView *view, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<Game> forIndex(const QModelIndex &index) const;
};

#endif // GAMESDELEGATE_H
