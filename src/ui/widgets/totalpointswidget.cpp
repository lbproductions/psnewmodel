#include "totalpointswidget.h"
#include "ui_totalpointswidget.h"

#include <QVBoxLayout>
#include <QPushButton>

#include <data/game.h>
#include <data/player.h>
#include <data/round.h>
#include <data/league.h>
#include <data/playerstatistics.h>

#include <misc/tools.h>

TotalPointsWidget::TotalPointsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TotalPointsWidget)
{
    ui->setupUi(this);

    this->setPalette(Tools::darkPalette(this));

    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonTotalPoints);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonWinRoundPoints);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonRoundWins);

    ui->treeWidgetTotalPoints->setPalette(Tools::darkPalette(ui->treeWidgetTotalPoints));
    ui->treeWidgetRoundWinPoints->setPalette(Tools::darkPalette(ui->treeWidgetRoundWinPoints));
    ui->treeWidgetRoundWins->setPalette(Tools::darkPalette(ui->treeWidgetRoundWins));


    ui->buttonGroup->setId(ui->pushButtonTotalPoints, 0);
    ui->buttonGroup->setId(ui->pushButtonWinRoundPoints, 1);
    ui->buttonGroup->setId(ui->pushButtonRoundWins, 2);
    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
}

void TotalPointsWidget::setGames(QList<QSharedPointer<Game> > games)
{
    QHash<QSharedPointer<Player>, int > points;
    QHash<QSharedPointer<Player>, double > roundWinPoints;

    foreach(QSharedPointer<Game> game, games) {
        foreach(QSharedPointer<Player> player, game->players()) {
            points.insert(player, points.value(player) + game->totalPoints(player));
        }
    }

    foreach(QSharedPointer<Player> player, points.keys()) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidgetTotalPoints);
        item->setIcon(0, QIcon(player->colorPixmap()));
        item->setText(1, player->name());
        item->setData(2, Qt::DisplayRole, points.value(player));
        item->setTextAlignment(2, Qt::AlignRight);
        ui->treeWidgetTotalPoints->addTopLevelItem(item);
    }

    ui->treeWidgetTotalPoints->resizeColumnToContents(0);
    ui->treeWidgetTotalPoints->resizeColumnToContents(1);
    ui->treeWidgetTotalPoints->resizeColumnToContents(2);
    ui->treeWidgetTotalPoints->sortByColumn(2);
    ui->treeWidgetTotalPoints->setMinimumWidth(175);



}

void TotalPointsWidget::setLeague(QSharedPointer<League> league)
{
    setGames(league->calculatedGames());

    foreach(QSharedPointer<Player> player, league->players()) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidgetRoundWinPoints);
        item->setIcon(0, QIcon(player->colorPixmap()));
        item->setText(1, player->name());
        item->setData(2, Qt::DisplayRole, league->playerStats(player)->winRounds().size());
        item->setData(3, Qt::DisplayRole, league->playerStats(player)->averagePointsPerRound());
        item->setTextAlignment(2, Qt::AlignRight);
        item->setTextAlignment(3, Qt::AlignRight);
        ui->treeWidgetRoundWinPoints->addTopLevelItem(item);

        QTreeWidgetItem* roundWinItem = new QTreeWidgetItem(ui->treeWidgetRoundWins);
        roundWinItem->setIcon(0, QIcon(player->colorPixmap()));
        roundWinItem->setText(1, player->name());
        roundWinItem->setData(2, Qt::DisplayRole, league->playerStats(player)->winRounds().size());
        roundWinItem->setData(3, Qt::DisplayRole, league->playerStats(player)->rounds().size());
        roundWinItem->setData(4, Qt::DisplayRole, league->playerStats(player)->roundWinsPercentage());
        roundWinItem->setTextAlignment(4, Qt::AlignRight);
        ui->treeWidgetRoundWins->addTopLevelItem(roundWinItem);
    }

    ui->treeWidgetRoundWinPoints->resizeColumnToContents(0);
    ui->treeWidgetRoundWinPoints->setColumnWidth(1, 150);
    ui->treeWidgetRoundWinPoints->resizeColumnToContents(2);
    ui->treeWidgetRoundWinPoints->resizeColumnToContents(3);
    ui->treeWidgetRoundWinPoints->sortByColumn(3);

    ui->treeWidgetRoundWins->resizeColumnToContents(0);
    ui->treeWidgetRoundWins->setColumnWidth(1, 150);
    ui->treeWidgetRoundWins->resizeColumnToContents(2);
    ui->treeWidgetRoundWins->resizeColumnToContents(3);
    ui->treeWidgetRoundWins->resizeColumnToContents(4);
    ui->treeWidgetRoundWins->sortByColumn(4);
}
