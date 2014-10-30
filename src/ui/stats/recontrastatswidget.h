#ifndef RECONTRASTATSWIDGET_H
#define RECONTRASTATSWIDGET_H

#include <QWidget>
#include <data/game.h>

namespace Ui {
class ReContraStatsWidget;
}

class PlayerStatistics;
class League;

class ReContraStatsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReContraStatsWidget(QWidget *parent = 0);
    ~ReContraStatsWidget();

    void setGame(QSharedPointer<Game> game);
    void setGames(QList<QSharedPointer<Game> > list);
    void setGames(QList<QSharedPointer<Game> > list, QList<QSharedPointer<Player> > players);
    void setLeague(QSharedPointer<League> league);

private:
    Ui::ReContraStatsWidget *ui;
    QList<QSharedPointer<Game> > m_games;

    QHash<QSharedPointer<Player>, PlayerStatistics*> m_playerStats;
};

bool sortPlayersByRe(QSharedPointer<Player> player, QSharedPointer<Player> player1);

#endif // RECONTRASTATSWIDGET_H
