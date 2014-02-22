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

class GameStatsWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit GameStatsWidget(QWidget *parent = 0);
        ~GameStatsWidget();

        void setGames(QList<QSharedPointer<Game> > games);
        void setLeague(QSharedPointer<League> league);

   private slots:
        void onItemClicked(QTreeWidgetItem* item);
        
    private:
        void addWidget(QString name, QWidget* widget);

        Ui::GameStatsWidget *ui;

        QList<QSharedPointer<Game> > m_games;

        QHash<QTreeWidgetItem*, int> m_indexes;

        ReContraStatsWidget* m_reContraWidget;
        PointsStatsWidget* m_pointsStatsWidget;
        GamesTogetherWidget* m_gamesTogetherWidget;
        SoloOverviewWidget* m_soloWidget;
        DrinkStatsWidget* m_drinksWidget;
        GameCompareStatsWidget* m_gameCompareStatsWidget;
        TimeStatsWidget* m_timeStatsWidget;

        int m_widgetCounter;
};

#endif // GAMESTATSWIDGET_H
