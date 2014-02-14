#include "gamecomparestatswidget.h"
#include "ui_gamecomparestatswidget.h"

#include <misc/tools.h>

#include <data/league.h>
#include <data/game.h>
#include <data/gamecomparestatistics.h>

#include <QDebug>

GameCompareStatsWidget::GameCompareStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameCompareStatsWidget)
{
    ui->setupUi(this);

    ui->comboBox->addItem(tr("All games"));

    this->setPalette(Tools::darkPalette(this));
    ui->treeWidget->setPalette(Tools::darkPalette(ui->treeWidget));
    ui->treeWidget->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());

    ui->treeWidget->setAttribute(Qt::WA_MacShowFocusRect, false);

    ui->treeWidget->setHeaderLabels(QStringList() << tr("Game") << tr("Hochzeit") << tr("Solo") << tr("Pflichtsolo") << tr("Trumpfabgabe")
                                    << tr("Schweinerei") << tr("Schmeisserei"));

    connect(ui->spinBoxRound, SIGNAL(valueChanged(int)), this, SLOT(update()));
}

GameCompareStatsWidget::~GameCompareStatsWidget()
{
    delete ui;
}

void GameCompareStatsWidget::setLeague(QSharedPointer<League> league)
{
    ui->comboBox->addItem(league->name());

    m_league = league;

    update();
}

void GameCompareStatsWidget::selectGame(QSharedPointer<Game> game)
{
    m_selectedGame = game;
}

void GameCompareStatsWidget::on_radioButtonPlacement_clicked(bool checked)
{
    if(checked) {
        ui->radioButtonValue->setChecked(false);
    }

    update();
}

void GameCompareStatsWidget::on_radioButtonValue_clicked(bool checked)
{
    if(checked) {
        ui->radioButtonPlacement->setChecked(false);
    }

    update();
}

void GameCompareStatsWidget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == tr("All games")) {
        GameCompareStatistics::instance().resetFilter();
    }
    else if(arg1 == m_league->name()){
        GameCompareStatistics::instance().filterGames(m_league->calculatedGames());
    }

    update();
}

void GameCompareStatsWidget::update()
{
    if(!this->isVisible()) {
       return;
    }



    QList<QSharedPointer<Game> > gameList = GameCompareStatistics::instance().games();
    qDebug() << "GameCompareStatsWidget::update(): GameList.size(): " << gameList.size();

    ui->treeWidget->clear();

    foreach(QSharedPointer<Game> game, gameList) {
        if(game->rounds().size() >= ui->spinBoxRound->value()) {
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
            item->setText(0, game->name());
            if(ui->radioButtonPlacement->isChecked()) {
                item->setData(1, Qt::DisplayRole, GameCompareStatistics::instance().placementOfGame(game, Slot::Hochzeit, ui->spinBoxRound->value()-1));
                item->setData(2, Qt::DisplayRole, GameCompareStatistics::instance().placementOfGame(game, Slot::Solo, ui->spinBoxRound->value()-1));
                item->setData(3, Qt::DisplayRole, GameCompareStatistics::instance().placementOfGame(game, Slot::Pflichtsolo, ui->spinBoxRound->value()-1));
                item->setData(4, Qt::DisplayRole, GameCompareStatistics::instance().placementOfGame(game, Slot::Trumpfabgabe, ui->spinBoxRound->value()-1));
                item->setData(5, Qt::DisplayRole, GameCompareStatistics::instance().placementOfGame(game, Slot::Schweinerei, ui->spinBoxRound->value()-1));
                item->setData(6, Qt::DisplayRole, GameCompareStatistics::instance().placementOfGame(game, Slot::Schmeisserei, ui->spinBoxRound->value()-1));
            }
            else {
                item->setData(1, Qt::DisplayRole, GameCompareStatistics::instance().value(game, Slot::Hochzeit, ui->spinBoxRound->value()-1));
                item->setData(2, Qt::DisplayRole, GameCompareStatistics::instance().value(game, Slot::Solo, ui->spinBoxRound->value()-1));
                item->setData(3, Qt::DisplayRole, GameCompareStatistics::instance().value(game, Slot::Pflichtsolo, ui->spinBoxRound->value()-1));
                item->setData(4, Qt::DisplayRole, GameCompareStatistics::instance().value(game, Slot::Trumpfabgabe, ui->spinBoxRound->value()-1));
                item->setData(5, Qt::DisplayRole, GameCompareStatistics::instance().value(game, Slot::Schweinerei, ui->spinBoxRound->value()-1));
                item->setData(6, Qt::DisplayRole, GameCompareStatistics::instance().value(game, Slot::Schmeisserei, ui->spinBoxRound->value()-1));
            }
            ui->treeWidget->addTopLevelItem(item);
        }
    }

    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->sortByColumn(0);

    for(int column = 1; column < ui->treeWidget->columnCount(); column++) {
        if(ui->radioButtonPlacement->isChecked()) {
            ui->treeWidget->sortByColumn(column, Qt::AscendingOrder);
        }
        else {
            ui->treeWidget->sortByColumn(column, Qt::DescendingOrder);
        }
        for(int i = 0; i<ui->treeWidget->topLevelItemCount(); i++) {
            if(ui->treeWidget->topLevelItem(i)->data(column, Qt::DisplayRole).toDouble() == ui->treeWidget->topLevelItem(0)->data(column, Qt::DisplayRole).toDouble()) {
                ui->treeWidget->topLevelItem(i)->setBackgroundColor(column, QColor(0, 123, 0));
            }
        }
    }
}

void GameCompareStatsWidget::showEvent(QShowEvent *)
{
    update();
}
