#ifndef LEAGUEWINDOW_H
#define LEAGUEWINDOW_H

#include <QMainWindow>

class League;
class LeagueClassementModel;
class LeagueGamePlacementModel;

namespace Ui {
    class LeagueWindow;
}

class LeagueWindow : public QMainWindow
{
        Q_OBJECT
    public:
        explicit LeagueWindow(QWidget *parent = 0);
        ~LeagueWindow();

        void setLeague(QSharedPointer<League> league);

        void wheelEvent(QWheelEvent *e);
        
    signals:
        
    public slots:

    private slots:
        void on_pushButtonCurrent_clicked();

        void on_pushButtonPrev_clicked();

        void on_pushButtonNext_clicked();

        void on_comboBoxAverage_currentIndexChanged(const QString &arg1);

    private:
        Ui::LeagueWindow *ui;

        QSharedPointer<League> m_league;

        LeagueClassementModel* m_classementModel;
        LeagueGamePlacementModel* m_gamePlacementModel;
        
};

#endif // LEAGUEWINDOW_H
