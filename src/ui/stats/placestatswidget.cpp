#include "placestatswidget.h"
#include "ui_placestatswidget.h"

#include "data/league.h"
#include "data/player.h"
#include "data/game.h"
#include "data/place.h"

#include <QLabel>
#include <QProgressBar>
#include <misc/tools.h>

PlaceStatsWidget::PlaceStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaceStatsWidget)
{
    ui->setupUi(this);

    ui->scrollAreaWidgetContents->setStyleSheet("QWidget {color:white; background-color: rgb(55,55,55);}");

    ui->groupBoxWins->setPalette(Tools::darkPalette(ui->groupBoxWins));
    ui->groupBoxLosses->setPalette(Tools::darkPalette(ui->groupBoxLosses));
    ui->groupBoxAverage->setPalette(Tools::darkPalette(ui->groupBoxAverage));
}

PlaceStatsWidget::~PlaceStatsWidget()
{
    delete ui;
}

void PlaceStatsWidget::setLeague(QSharedPointer<League> league)
{
    m_league = league;
    connect(m_league.data(), SIGNAL(recalculatedData()), this, SLOT(update()));

    ui->playerComboBox->addPlayers(league->players());

    ui->playerComboBox->setCurrentIndex(1);
}

void PlaceStatsWidget::on_playerComboBox_currentIndexChanged(int index)
{
    clearLayouts();

    QSharedPointer<Player> player = ui->playerComboBox->currentPlayer();
    if(!player) {
        return;
    }
    QHash<QSharedPointer<Place>,PlaceStats*> stats;

    foreach(QSharedPointer<Game> game, m_league->filteredGames()) {
        if(game->players().contains(player)) {
            PlaceStats* placeStats = stats.value(game->site());
            if(!placeStats) {
                placeStats = new PlaceStats(this);
                placeStats->games = 0;
                placeStats->wins = 0;
                placeStats->losses = 0;
                placeStats->average = 0;
                placeStats->points = 0;
            }
            placeStats->games++;

            int playerCount = game->players().size();
            int zaehler = playerCount - game->placement(player);
            int nenner = playerCount - 1;
            placeStats->points += 100 * zaehler / nenner;

            if(game->placement(player) == 1) {
                placeStats->wins++;
            }
            if(game->placement(player) == game->players().size()) {
                placeStats->losses++;
            }
            stats.insert(game->site(), placeStats);
        }
    }

    QGridLayout* winsLayout = static_cast<QGridLayout*>(ui->groupBoxWins->layout());
    if(!winsLayout) {
        winsLayout = new QGridLayout(ui->groupBoxWins);
        ui->groupBoxWins->setLayout(winsLayout);
    }

    QGridLayout* lossesLayout = static_cast<QGridLayout*>(ui->groupBoxLosses->layout());
    if(!lossesLayout) {
        lossesLayout = new QGridLayout(ui->groupBoxLosses);
        ui->groupBoxLosses->setLayout(lossesLayout);
    }

    QGridLayout* avgLayout = static_cast<QGridLayout*>(ui->groupBoxAverage->layout());
    if(!avgLayout) {
        avgLayout = new QGridLayout(ui->groupBoxAverage);
        ui->groupBoxAverage->setLayout(avgLayout);
    }

    foreach(QSharedPointer<Place> place, stats.keys()) {
        PlaceStats* placeStats = stats.value(place);
        placeStats->average = (double)placeStats->points / (double)placeStats->games;

        if(placeStats->wins > 0) {
            QLabel* name = new QLabel(place->street(), this);
            winsLayout->addWidget(name, winsLayout->rowCount(), 0);

            QProgressBar* bar = new QProgressBar(this);
            double perc = placeStats->wins * 100 / placeStats->games;
            bar->setValue(perc);
            bar->setAlignment(Qt::AlignCenter);
            bar->setStyleSheet(Tools::progressBarStyleSheet(QColor(255,165,0)));
            bar->setTextVisible(true);
            bar->setFormat(QString::number(perc)+"% (" + QString::number(placeStats->wins) + " / " + QString::number(placeStats->games) + ")");
            winsLayout->addWidget(bar, winsLayout->rowCount(), 1);
        }

        if(placeStats->losses > 0) {
            QLabel* name = new QLabel(place->street(), this);
            lossesLayout->addWidget(name, lossesLayout->rowCount(), 0);

            QProgressBar* bar = new QProgressBar(this);
            double perc = placeStats->losses * 100 / placeStats->games;
            bar->setValue(perc);
            bar->setAlignment(Qt::AlignCenter);
            bar->setStyleSheet(Tools::progressBarStyleSheet(QColor(255,165,0)));
            bar->setTextVisible(true);
            bar->setFormat(QString::number(perc)+"% (" + QString::number(placeStats->losses) + " / " + QString::number(placeStats->games) + ")");
            lossesLayout->addWidget(bar, lossesLayout->rowCount(), 1);
        }

        QLabel* name = new QLabel(place->street(), this);
        avgLayout->addWidget(name, avgLayout->rowCount(), 0);

        QProgressBar* bar = new QProgressBar(this);
        bar->setValue(placeStats->average);
        bar->setAlignment(Qt::AlignCenter);
        bar->setStyleSheet(Tools::progressBarStyleSheet(QColor(255,165,0)));
        bar->setTextVisible(true);
        bar->setFormat(QString::number(placeStats->average) + " (" + QString::number(placeStats->games) + " games)");
        avgLayout->addWidget(bar, avgLayout->rowCount(), 1);
    }
}

void PlaceStatsWidget::clearLayouts()
{
    clearLayout(ui->groupBoxWins->layout());
    clearLayout(ui->groupBoxLosses->layout());
    clearLayout(ui->groupBoxAverage->layout());
}

void PlaceStatsWidget::update()
{
    on_playerComboBox_currentIndexChanged(ui->playerComboBox->currentIndex());
}

void PlaceStatsWidget::clearLayout(QLayout* layout, bool deleteWidgets)
{
    if(layout == 0) {
        return;
    }

    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                delete widget;
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}


PlaceStats::PlaceStats(QObject *parent):
    QObject(parent)
{}
