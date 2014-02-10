#ifndef OVERVIEWHORIZONTALHEADERVIEW_H
#define OVERVIEWHORIZONTALHEADERVIEW_H

#include <QHeaderView>

class OverviewHorizontalHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit OverviewHorizontalHeaderView(Qt::Orientation orientation, QWidget *parent = 0);

    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const Q_DECL_OVERRIDE;
    
};

#endif // OVERVIEWHORIZONTALHEADERVIEW_H
