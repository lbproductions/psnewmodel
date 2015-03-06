#ifndef SOLOOVERVIEWWIDGET_H
#define SOLOOVERVIEWWIDGET_H

#include <QWidget>

#include <data/round.h>

namespace Ui {
    class SoloOverviewWidget;
}

class Game;
class Player;
class League;
class PlayerStatistics;

class SoloOverviewWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit SoloOverviewWidget(QWidget *parent = 0);
        ~SoloOverviewWidget();

        void setGames(QList<QSharedPointer<Game> > games);
        void setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players);
        void setLeague(QSharedPointer<League> league);

        void init();
        
    private:
        void createCountHash();

        int totalTypeCount(Round::SoloType type);
        int totalCount();

        Ui::SoloOverviewWidget *ui;

        QList<QSharedPointer<Player> > m_players;

        QHash<QSharedPointer<Player>, QSharedPointer<PlayerStatistics>> m_stats;
};

#endif // SOLOOVERVIEWWIDGET_H
