#ifndef GAMESTATSWIDGET_H
#define GAMESTATSWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>


namespace Ui {
    class GameStatsWidget;
}

class Game;
class League;
class ReContraStatsWidget;
class PointsStatsWidget;
class GamesTogetherWidget;
class SoloOverviewWidget;
class DrinkStatsWidget;
class GameCompareStatsWidget;
class TimeStatsWidget;
class ServeStatsWidget;
class SeriesStatsWidget;
class PlaceStatsWidget;

class GamesWidget;

class GameStatsWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit GameStatsWidget(QWidget *parent = 0);
        ~GameStatsWidget();

        void setGames(QList<QSharedPointer<Game> > games);
        void setLeague(QSharedPointer<League> _league);

   private slots:
        
    private:
        void addWidget(QWidget* widget, QString name, QIcon icon = QIcon());
        void insertWidget(int index, QString name, QWidget* widget);

        void resetAllButtons();

        Ui::GameStatsWidget *ui;

        QList<QSharedPointer<Game> > m_games;

        ReContraStatsWidget* m_reContraWidget;
        PointsStatsWidget* m_pointsStatsWidget;
        GamesTogetherWidget* m_gamesTogetherWidget;
        SoloOverviewWidget* m_soloWidget;
        DrinkStatsWidget* m_drinksWidget;
        GameCompareStatsWidget* m_gameCompareStatsWidget;
        TimeStatsWidget* m_timeStatsWidget;
        ServeStatsWidget* m_serveStatsWidget;
        SeriesStatsWidget* m_seriesStatsWidget;
        PlaceStatsWidget* m_placeStatsWidget;

        GamesWidget* m_gamesWidget;

        int m_widgetCounter;
};

#endif // GAMESTATSWIDGET_H
