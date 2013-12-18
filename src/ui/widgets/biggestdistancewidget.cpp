#include "biggestdistancewidget.h"
#include "ui_biggestdistancewidget.h"

#include <data/game.h>
#include <data/player.h>

#include <ui/game/gamewindow.h>

#include <misc/tools.h>

BiggestDistanceWidget::BiggestDistanceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BiggestDistanceWidget)
{
    ui->setupUi(this);

    this->setPalette(Tools::darkPalette(this));

    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonLoser);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonWinner);

    ui->treeWidgetLoser->setPalette(Tools::darkPalette(ui->treeWidgetLoser));
    ui->treeWidgetWinner->setPalette(Tools::darkPalette(ui->treeWidgetWinner));

    ui->buttonGroup->setId(ui->pushButtonWinner, 0);
    ui->buttonGroup->setId(ui->pushButtonLoser, 1);
    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));

    connect(ui->treeWidgetWinner, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*)));
    connect(ui->treeWidgetLoser, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*)));
}

BiggestDistanceWidget::~BiggestDistanceWidget()
{
    delete ui;
}

void BiggestDistanceWidget::setGames(QList<QSharedPointer<Game> > games)
{
    m_games = games;

    int gameIndex = 0;
    foreach(QSharedPointer<Game> game, games) {
        QSharedPointer<Player> loser = game->playersSortedByPlacement().last();
        QSharedPointer<Player> winner = game->playersSortedByPlacement().first();
        int distanceLast = game->totalPoints(loser) - game->totalPoints(game->playersSortedByPlacement().at(game->playersSortedByPlacement().size()-2));
        int distanceFirst = game->totalPoints(winner) - game->totalPoints(game->playersSortedByPlacement().at(1));

        QTreeWidgetItem* winnerItem = new QTreeWidgetItem(ui->treeWidgetWinner);
        winnerItem->setIcon(0, QIcon(winner->colorPixmap()));
        winnerItem->setText(1, winner->name());
        winnerItem->setData(2, Qt::DisplayRole, distanceFirst);
        winnerItem->setData(2, Qt::UserRole+1, gameIndex);
        winnerItem->setTextAlignment(2, Qt::AlignRight);
        ui->treeWidgetWinner->addTopLevelItem(winnerItem);


        QTreeWidgetItem* loserItem = new QTreeWidgetItem(ui->treeWidgetLoser);
        loserItem->setIcon(0, QIcon(loser->colorPixmap()));
        loserItem->setText(1, loser->name());
        loserItem->setData(2, Qt::DisplayRole, distanceLast);
        loserItem->setData(2, Qt::UserRole+1, gameIndex);
        loserItem->setTextAlignment(2, Qt::AlignRight);
        ui->treeWidgetLoser->addTopLevelItem(loserItem);

        gameIndex++;
    }

    ui->treeWidgetWinner->resizeColumnToContents(0);
    ui->treeWidgetWinner->resizeColumnToContents(2);
    ui->treeWidgetWinner->setColumnWidth(1,150);
    ui->treeWidgetWinner->sortByColumn(2);

    ui->treeWidgetLoser->resizeColumnToContents(0);
    ui->treeWidgetLoser->resizeColumnToContents(2);
    ui->treeWidgetLoser->setColumnWidth(1,150);
    ui->treeWidgetLoser->sortByColumn(2, Qt::AscendingOrder);
}

void BiggestDistanceWidget::onItemDoubleClicked(QTreeWidgetItem *item)
{
    int index = item->data(2, Qt::UserRole+1).toInt();
    if(index < 0 || index >= m_games.size())
        return;

    QSharedPointer<Game> game = m_games.at(index);
    if(!game)
        return;

    GameWindow *window = new GameWindow(/*this*/);
    window->setGame(game);
    window->show();
}
