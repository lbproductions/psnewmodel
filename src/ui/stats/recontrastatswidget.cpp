#include "recontrastatswidget.h"
#include "ui_recontrastatswidget.h"

#include <QProgressBar>

#include <data/player.h>
#include <data/playerstatistics.h>
#include <data/league.h>
#include <misc/tools.h>

#include "recontraplayerstatswidget.h"
#include "recontrastatslegendwidget.h"
#include "recontrageneralstatswidget.h"

ReContraStatsWidget::ReContraStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReContraStatsWidget)
{
    ui->setupUi(this);

    ui->scrollAreaWidgetContents->setStyleSheet("QWidget {color:white; background-color: rgb(55,55,55);}");

    //ui->groupBoxRe->setPalette(Tools::darkPalette(ui->groupBoxRe));
    //ui->groupBoxReWins->setPalette(Tools::darkPalette(ui->groupBoxReWins));
    //ui->groupBoxContraWins->setPalette(Tools::darkPalette(ui->groupBoxContraWins));
}

ReContraStatsWidget::~ReContraStatsWidget()
{
    delete ui;
}

void ReContraStatsWidget::setGame(QSharedPointer<Game> game)
{
    if(!game)
        return;

    setGames(QList<QSharedPointer<Game> >() << game);
}

void ReContraStatsWidget::setGames(QList<QSharedPointer<Game> > list)
{
    QList<QSharedPointer<Player> > players;
    foreach(QSharedPointer<Game> game, list) {
        foreach(QSharedPointer<Player> player, game->players()) {
            if(!players.contains(player)) {
                players.append(player);
            }
        }
    }

    setGames(list, players);
}

void ReContraStatsWidget::setGames(QList<QSharedPointer<Game> > list, QList<QSharedPointer<Player> > players)
{
    m_games = list;

    int reWins = 0;
    int contraWins = 0;

    int reGamePoints = 0;
    int contraGamePoints = 0;
    int rounds = 0;

    foreach(QSharedPointer<Game> game, m_games) {

        foreach(QSharedPointer<Player> player, players) {
            if(!m_playerStats.contains(player)) {
                PlayerStatistics* stats = new PlayerStatistics(this);
                stats->setPlayer(player.data());
                stats->setGames(m_games);
                m_playerStats.insert(player, stats);
            }
        }

        reWins += game->reWinsCount();
        contraWins += game->contraWinCount();

        if(game->creationTime().date() < QDate(2014,1,21)) {
            continue;
        }
        reGamePoints += game->reGamePoints();
        contraGamePoints += game->contraGamePoints();
        rounds += game->finishedRoundCount();
    }

    ReContraGeneralStatsWidget* generalWidget = new ReContraGeneralStatsWidget(this);
    generalWidget->setReRoundWins(reWins);
    generalWidget->setContraRoundWins(contraWins);
    generalWidget->setReGamePointAverage(reGamePoints / rounds);
    generalWidget->setContraGamePointAverage(contraGamePoints / rounds);
    ui->verticalLayoutPlayers->addWidget(generalWidget);

    ReContraStatsLegendWidget* legend = new ReContraStatsLegendWidget(this);
    ui->verticalLayoutPlayers->addWidget(legend);
    foreach(QSharedPointer<Player> player, m_playerStats.keys()) {
        ReContraPlayerStatsWidget* widget = new ReContraPlayerStatsWidget(this);
        widget->setPlayer(player);
        widget->setRePercentage(m_playerStats.value(player)->rePercentage());
        widget->setReWinPercentage(m_playerStats.value(player)->reWinsPercentage());
        widget->setContraWinPercentage(m_playerStats.value(player)->contraWinsPercentage());
        ui->verticalLayoutPlayers->addWidget(widget);
    }

    //fillReBox();
    //fillReWinsBox();
    //fillContraWinsBox();
}

void ReContraStatsWidget::setLeague(QSharedPointer<League> league)
{
    setGames(league->calculatedGames(), league->players());
}

/*

void ReContraStatsWidget::fillReBox()
{
    QGridLayout* layout = new QGridLayout(this);
    int row = 0;
    foreach(QSharedPointer<Player> player, m_playerStats.keys()) {
        layout->addWidget(new QLabel(player->name(), this), row, 0);
        QProgressBar* bar = new QProgressBar(this);
        int rePercentage = m_playerStats.value(player)->rePercentage();
        bar->setValue(rePercentage);
        bar->setAlignment(Qt::AlignCenter);
        bar->setStyleSheet(Tools::progressBarStyleSheet(percentageColor(rePercentage)));
        layout->addWidget(bar, row, 1);
        row++;
    }

    ui->groupBoxRe->setLayout(layout);
}

void ReContraStatsWidget::fillReWinsBox()
{
    QGridLayout* layout = new QGridLayout(this);
    int row = 0;
    foreach(QSharedPointer<Player> player, m_playerStats.keys()) {
        layout->addWidget(new QLabel(player->name(), this), row, 0);
        QProgressBar* bar = new QProgressBar(this);
        int reWinPercentage = m_playerStats.value(player)->reWinsPercentage();
        bar->setValue(reWinPercentage);
        bar->setAlignment(Qt::AlignCenter);
        bar->setStyleSheet(Tools::progressBarStyleSheet(percentageColor(reWinPercentage)));
        layout->addWidget(bar, row, 1);
        row++;
    }

    ui->groupBoxReWins->setLayout(layout);
}

void ReContraStatsWidget::fillContraWinsBox()
{
    QGridLayout* layout = new QGridLayout(this);
    int row = 0;
    foreach(QSharedPointer<Player> player, m_playerStats.keys()) {
        layout->addWidget(new QLabel(player->name(), this), row, 0);
        QProgressBar* bar = new QProgressBar(this);
        int contraPercentage = m_playerStats.value(player)->contraWinsPercentage();
        bar->setValue(contraPercentage);
        bar->setAlignment(Qt::AlignCenter);
        bar->setStyleSheet(Tools::progressBarStyleSheet(percentageColor(contraPercentage)));
        layout->addWidget(bar, row, 1);
        row++;
    }

    ui->groupBoxContraWins->setLayout(layout);
}
*/
