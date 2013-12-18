#ifndef LEAGUEPLAYERHEADERVIEW_H
#define LEAGUEPLAYERHEADERVIEW_H

#include <QHeaderView>

class LeagueClassementModel;

class LeaguePlayerHeaderView: public QHeaderView
{
    public:
        LeaguePlayerHeaderView(Qt::Orientation orientation, QWidget *parent = 0);

        LeagueClassementModel *model() const;
        void setLeagueClassementModel(LeagueClassementModel *model);

    protected:
        QSize sizeHint() const Q_DECL_OVERRIDE;
        QSize sectionSizeFromContents(int logicalIndex) const Q_DECL_OVERRIDE;
        void paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const Q_DECL_OVERRIDE;

    private:
        LeagueClassementModel* m_model;
};

#endif // LEAGUEPLAYERHEADERVIEW_H
