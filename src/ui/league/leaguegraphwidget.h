#ifndef LEAGUEGRAPHWIDGET_H
#define LEAGUEGRAPHWIDGET_H

#include <ui/widgets/graphwidget.h>

class Matchday;
class League;

class LeagueGraphWidget : public GraphWidget
{
        Q_OBJECT
    public:
        enum GraphType {
          PlacementGraph,
          AverageGraph,
          PointsGraph,
          TotalPointsGraph
        };

        explicit LeagueGraphWidget(QWidget *parent = 0);

        void setLeague(QSharedPointer<League> league);

        QSize sizeHint() const;

        int translateX(int x) const;

        GraphType type();
        void setType(GraphType type);
        
    signals:
        void graphUpdated();
        
    public slots:
        void updateGraphs();

    private:
        void paintEvent(QPaintEvent *event);

        QSharedPointer<League> m_league;

        QList<QList<int> > m_matchdays;
        QList<QList<QColor> > m_colors;


        GraphType m_type;
        
};

#endif // LEAGUEGRAPHWIDGET_H
