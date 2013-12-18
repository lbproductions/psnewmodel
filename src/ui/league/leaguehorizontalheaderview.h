#ifndef LEAGUEHORIZONTALHEADERVIEW_H
#define LEAGUEHORIZONTALHEADERVIEW_H

#include <QHeaderView>

class LeagueHorizontalHeaderView : public QHeaderView
{
    public:
        LeagueHorizontalHeaderView(Qt::Orientation orientation, QWidget *parent = 0);

    protected:
        void paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const Q_DECL_OVERRIDE;
};

#endif // LEAGUEHORIZONTALHEADERVIEW_H
