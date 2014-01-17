#include "recontrastatswidget.h"
#include "ui_recontrastatswidget.h"

#include <QProgressBar>

#include <data/player.h>
#include <data/playerstatistics.h>
#include <misc/tools.h>

ReContraStatsWidget::ReContraStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReContraStatsWidget)
{
    ui->setupUi(this);

    this->setStyleSheet("QLabel{color:white;}");

    ui->groupBoxRe->setPalette(Tools::darkPalette(ui->groupBoxRe));
    ui->groupBoxReWins->setPalette(Tools::darkPalette(ui->groupBoxReWins));
    ui->groupBoxContraWins->setPalette(Tools::darkPalette(ui->groupBoxContraWins));
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
    m_games = list;

    int reWins = 0;
    int contraWins = 0;

    foreach(QSharedPointer<Game> game, m_games) {

        foreach(QSharedPointer<Player> player, game->players()) {
            if(!m_playerStats.contains(player)) {
                PlayerStatistics* stats = new PlayerStatistics(this);
                stats->setPlayer(player.data());
                stats->setGames(m_games);
                m_playerStats.insert(player, stats);
            }
        }

        reWins += game->reWinsCount();
        contraWins += game->contraWinCount();
    }

    ui->labelReWins->setText(QString::number(reWins));
    ui->labelContraWins->setText(QString::number(contraWins));

    fillReBox();
    fillReWinsBox();
    fillContraWinsBox();
}

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

QColor ReContraStatsWidget::percentageColor(double percentage)
{
    if(percentage < 25) {
        return QColor("red");
    }
    else if(percentage < 50) {
        return QColor(123,0,0);
    }
    else if(percentage < 75) {
        return QColor(0, 123, 0);
    }
    else {
        return QColor(0,200,0);
    }
}
