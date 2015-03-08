#ifndef DRINKSDELEGATE_H
#define DRINKSDELEGATE_H

#include "noisygradientitemdelegate.h"

class Drink;

class DrinksDelegate : public NoisyGradientItemDelegate
{
public:
    explicit DrinksDelegate(QAbstractItemView *view, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<Drink> forIndex(const QModelIndex &index) const;
};

#endif // DRINKSDELEGATE_H
