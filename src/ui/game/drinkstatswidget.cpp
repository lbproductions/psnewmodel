#include "drinkstatswidget.h"
#include "ui_drinkstatswidget.h"

#include <misc/tools.h>
#include <data/game.h>
#include <data/livedrink.h>
#include <data/drink.h>
#include <data/player.h>

DrinkStatsWidget::DrinkStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrinkStatsWidget)
{
    ui->setupUi(this);

    this->setPalette(Tools::darkPalette(this));
    ui->treeWidget->setPalette(Tools::darkPalette(ui->treeWidget));
    ui->treeWidget->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());

    ui->treeWidget->setHeaderLabels(QStringList() << tr("Player") << tr("DrinkCount") << tr("DrinksVolume") << tr("AlcVolume"));
}

DrinkStatsWidget::~DrinkStatsWidget()
{
    delete ui;
}

void DrinkStatsWidget::setGames(QList<QSharedPointer<Game> > games)
{
    QList<QSharedPointer<LiveDrink> > drinks;
    foreach(QSharedPointer<Game> game, games) {
        drinks.append(game->liveDrinks());
    }

    QHash<QSharedPointer<Player>, double> playerVolume;
    QHash<QSharedPointer<Player>, QList<QSharedPointer<LiveDrink> > > playerDrinks;

    double totalVolume = 0;
    foreach(QSharedPointer<LiveDrink> drink, drinks) {
        totalVolume += drink->drink()->volume();
        playerVolume.insert(drink->player(), playerVolume.value(drink->player()) + drink->drink()->volume());

        QList<QSharedPointer<LiveDrink> > list;
        if(playerDrinks.contains(drink->player())) {
            list.append(playerDrinks.value(drink->player()));
        }
        list.append(drink);
        playerDrinks.insert(drink->player(), list);
    }

    ui->labelTotalDrinkCount->setText(QString::number(drinks.size()));
    ui->labelTotalDrinkVolume->setText(QString::number(totalVolume) + " l");

    foreach(QSharedPointer<Player> player, playerDrinks.keys()) {
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
