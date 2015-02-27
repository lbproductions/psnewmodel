#ifndef LEAGUEWINDOW_H
#define LEAGUEWINDOW_H

#include <QMainWindow>

class League;
class LeagueClassementModel;
class LeagueGamePlacementModel;

class GameStatsWidget;

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

        void resizeEvent(QResizeEvent *);

        bool eventFilter(QObject *object, QEvent *event);

    signals:
        
    public slots:

    private slots:
        void on_comboBoxAverage_currentIndexChanged(const QString &arg1);

        void on_doubleSpinBoxFinishedGames_valueChanged(double arg1);

        void on_pushButtonStats_clicked();

        void on_tabWidget_currentChanged(int index);

private:
        Ui::LeagueWindow *ui;

        QSharedPointer<League> m_league;

        LeagueClassementModel* m_classementModel;
        LeagueGamePlacementModel* m_gamePlacementModel;

        GameStatsWidget* m_gameStatsWidget;

        int m_minimumColumnWidth;
        int m_maximumColumnWidth;
};

#endif // LEAGUEWINDOW_H
