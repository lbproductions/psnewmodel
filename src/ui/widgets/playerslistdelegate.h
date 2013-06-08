#ifndef PLAYERSLISTDELEGATE_H
#define PLAYERSLISTDELEGATE_H

#include <QStyledItemDelegate>

class PlayersListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PlayersListDelegate(QWidget *parent = 0);
    
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    
};

#endif // PLAYERSLISTDELEGATE_H
