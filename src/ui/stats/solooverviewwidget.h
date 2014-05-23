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

class SoloOverviewWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit SoloOverviewWidget(QWidget *parent = 0);
        ~SoloOverviewWidget();

        void setGames(QList<QSharedPointer<Game> > games);
        void setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players);
        void setLeague(QSharedPointer<League> league);
        
    private:
        void createCountHash();

        int totalTypeCount(Round::SoloType type);
        int totalPlayerCount(QSharedPointer<Player> player);

        Ui::SoloOverviewWidget *ui;

        QList<QSharedPointer<Game> > m_games;

        QList<QSharedPointer<Player> > m_players;

        QHash<Round::SoloType, QHash<QSharedPointer<Player>,int> > m_counts;
};

#endif // SOLOOVERVIEWWIDGET_H
