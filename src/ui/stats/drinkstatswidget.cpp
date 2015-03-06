#include "drinkstatswidget.h"
#include "ui_drinkstatswidget.h"

#include <misc/tools.h>
#include <data/game.h>
#include <data/livedrink.h>
#include <data/drink.h>
#include <data/player.h>
#include <data/league.h>

DrinkStatsWidget::DrinkStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrinkStatsWidget)
{
    ui->setupUi(this);

    this->setPalette(Tools::darkPalette(this));
    ui->treeWidget->setPalette(Tools::darkPalette(ui->treeWidget));
    ui->treeWidget->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());

    ui->treeWidget->setHeaderLabels(QStringList() << tr("Player") << tr("DrinkCount") << tr("DrinksVolume") << tr("AlcVolume"));

    ui->treeWidget->setAttribute(Qt::WA_MacShowFocusRect, false);
}

DrinkStatsWidget::~DrinkStatsWidget()
{
    delete ui;
}

void DrinkStatsWidget::setGames(QList<QSharedPointer<Game> > games)
{
    QList<QSharedPointer<Player> > players;
    foreach(QSharedPointer<Game> game, games) {
        foreach(QSharedPointer<Player> player, game->players()) {
            if(!players.contains(player)) {
                players.append(player);
            }
        }
    }

    setGames(games, players);
}

void DrinkStatsWidget::setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players)
{
    QList<QSharedPointer<LiveDrink> > drinks;
    foreach(QSharedPointer<Game> game, games) {
        drinks.append(game->liveDrinks());
    }

    QHash<QSharedPointer<Player>, double> playerVolume;
    QHash<QSharedPointer<Player>, QList<QSharedPointer<LiveDrink> > > playerDrinks;
    QHash<QSharedPointer<Drink>, int> drinkCounts;

    double totalVolume = 0;
    foreach(QSharedPointer<LiveDrink> drink, drinks) {
        totalVolume += drink->drink()->volume();
        playerVolume.insert(drink->player(), playerVolume.value(drink->player()) + drink->drink()->volume());

        QList<QSharedPointer<LiveDrink> > list;
        if(playerDrinks.contains(drink->player()) && players.contains(drink->player())) {
            list.append(playerDrinks.value(drink->player()));
        }
        list.append(drink);
        playerDrinks.insert(drink->player(), list);

        drinkCounts.insert(drink->drink(), drinkCounts.value(drink->drink()) + 1);
    }

    QList<QColor> colors = Tools::generateColors(drinkCounts.keys().size());
    foreach(QSharedPointer<Drink> drink, drinkCounts.keys()) {
        int count = drinkCounts.value(drink);
        double value = (double)count / (double)drinks.size() * 100.0;
        if(value <= 0) {
            continue;
        }
        ui->pieWidget->addValue(drink->name() + QString(" [%1]").arg(count), value, colors.at(drinkCounts.keys().indexOf(drink)%colors.size()));
    }

    ui->labelTotalDrinkCount->setText(QString::number(drinks.size()));
    ui->labelTotalDrinkVolume->setText(QString::number(totalVolume) + " l");

    foreach(QSharedPointer<Player> player, players) {
        double alc = 0;
        foreach(QSharedPointer<LiveDrink> drink, playerDrinks.value(player)) {
            alc += drink->drink()->alcoholByVolume()/100.0 * (double)drink->drink()->volume();
        }

        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, player->name());
        item->setIcon(0, QIcon(player->colorPixmap()));
        item->setData(1, Qt::DisplayRole, playerDrinks.value(player).size());
        item->setData(2, Qt::DisplayRole, playerVolume.value(player));
        item->setData(3, Qt::DisplayRole, alc);
    }

    ui->treeWidget->sortByColumn(0);

    for(int column = 1; column < ui->treeWidget->columnCount(); column++) {
        ui->treeWidget->sortByColumn(column);
        for(int i = 0; i<ui->treeWidget->topLevelItemCount(); i++) {
            if(ui->treeWidget->topLevelItem(i)->data(column, Qt::DisplayRole).toDouble() == ui->treeWidget->topLevelItem(0)->data(column, Qt::DisplayRole).toDouble()) {
                ui->treeWidget->topLevelItem(i)->setBackgroundColor(column, QColor(0, 123, 0));
            }
        }
    }
}

void DrinkStatsWidget::setLeague(QSharedPointer<League> league)
{
    setGames(league->calculatedGames(), league->players());
}
