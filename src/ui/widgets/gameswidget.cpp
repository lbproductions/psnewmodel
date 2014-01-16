#include "gameswidget.h"
#include "ui_gameswidget.h"

#include <misc/tools.h>
#include <data/game.h>
#include <data/place.h>
#include <ui/game/gamewindow.h>

GamesWidget::GamesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamesWidget)
{
    ui->setupUi(this);

    this->setPalette(Tools::darkPalette(this));

    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonGames);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonUnfinished);

    ui->treeWidget->setPalette(Tools::darkPalette(ui->treeWidget));
    ui->treeWidgetUnfinshed->setPalette(Tools::darkPalette(ui->treeWidgetUnfinshed));

    ui->buttonGroup->setId(ui->pushButtonGames, 0);
    ui->buttonGroup->setId(ui->pushButtonUnfinished, 1);
    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
}

GamesWidget::~GamesWidget()
{
    delete ui;
}

void GamesWidget::setGames(QList<QSharedPointer<Game> > games)
{
    m_games = games;

    foreach(QSharedPointer<Game> game, games) {
        ui->treeWidget->addTopLevelItem(createItem(ui->treeWidget, game->statePixmap(), game->name(), Tools::playersString(game->playersSortedByPlacement()).toString(),
                                                   game->creationTime().date().toString("dd.MM.yy"), game->site()->displayString(),
                                                   QString::number(game->completedPercentage()) + "%"));
        if(game->state() != Game::Finished) {
            ui->treeWidgetUnfinshed->addTopLevelItem(createItem(ui->treeWidgetUnfinshed, game->statePixmap(), game->name(), Tools::playersString(game->playersSortedByPlacement()).toString(),
                                                       game->creationTime().date().toString("dd.MM.yy"), game->site()->displayString(),
                                                       QString::number(game->completedPercentage()) + "%"));
            m_unfinishedGames.append(game);
        }
    }

    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
    ui->treeWidget->resizeColumnToContents(3);
    ui->treeWidget->resizeColumnToContents(4);

    ui->treeWidget->setMinimumWidth(660);

    ui->treeWidgetUnfinshed->resizeColumnToContents(0);
    ui->treeWidgetUnfinshed->resizeColumnToContents(1);
    ui->treeWidgetUnfinshed->resizeColumnToContents(2);
    ui->treeWidgetUnfinshed->resizeColumnToContents(3);
    ui->treeWidgetUnfinshed->resizeColumnToContents(4);

    ui->treeWidgetUnfinshed->setMinimumWidth(660);
}

void GamesWidget::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
    int index = ui->treeWidget->indexOfTopLevelItem(item);
    QSharedPointer<Game> game = m_games.at(index);
    if(!game)
        return;

    GameWindow *window = new GameWindow(/*this*/);
    window->setGame(game);
    window->show();
}

QTreeWidgetItem *GamesWidget::createItem(QTreeWidget* parent, QPixmap statePixmap, QString name, QString players, QString date, QString site, QString complete)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(parent);
    item->setIcon(0, QIcon(statePixmap));
    item->setText(1, name);
    item->setText(2, players);
    item->setText(3, date);
    item->setText(4, site);
    item->setText(5, complete);

    return item;
}

void GamesWidget::on_treeWidgetUnfinshed_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
    int index = ui->treeWidgetUnfinshed->indexOfTopLevelItem(item);
    if(index < 0 || index >= m_unfinishedGames.size())
        return;

    QSharedPointer<Game> game = m_unfinishedGames.at(index);
    if(!game)
        return;

    GameWindow *window = new GameWindow(/*this*/);
    window->setGame(game);
    window->show();
}
