#ifndef TOTALPOINTSWIDGET_H
#define TOTALPOINTSWIDGET_H

#include <QWidget>
#include <QTreeWidget>

namespace Ui {
    class TotalPointsWidget;
}

class Game;
class League;

class TotalPointsWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit TotalPointsWidget(QWidget *parent = 0);

        void setGames(QList<QSharedPointer<Game> > games);

        void setLeague(QSharedPointer<League> league);
        
    signals:
        
    public slots:

    private:
        Ui::TotalPointsWidget *ui;

        
};

#endif // TOTALPOINTSWIDGET_H
