#ifndef PLACESDELEGATE_H
#define PLACESDELEGATE_H

#include "noisygradientitemdelegate.h"

class Place;

class PlacesDelegate : public NoisyGradientItemDelegate
{
public:
    explicit PlacesDelegate(QAbstractItemView *view, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<Place> forIndex(const QModelIndex &index) const;
};

#endif // PLACESDELEGATE_H
