#ifndef OVERVIEWDELEGATE_H
#define OVERVIEWDELEGATE_H

#include <QStyledItemDelegate>

class OverviewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit OverviewDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    
};

#endif // OVERVIEWDELEGATE_H
