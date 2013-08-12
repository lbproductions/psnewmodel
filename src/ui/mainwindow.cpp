#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "playerinformationdialog.h"
#include "drinkinformationdialog.h"
#include "game/gamewindow.h"

#include <ui/widgets/menubar.h>
#include <model/playerslistmodel.h>
#include <model/gamelistmodel.h>
#include <model/drinkslistmodel.h>
#include <misc/tools.h>

#include <QApplication>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actionDrinks);
    actionGroup->addAction(ui->actionGames);
    actionGroup->addAction(ui->actionPlaces);
    actionGroup->addAction(ui->actionPlayers);
    actionGroup->setExclusive(true);
    ui->actionPlayers->setChecked(true);

    ui->toolButtonDrinks->setDefaultAction(ui->actionDrinks);
    ui->toolButtonGames->setDefaultAction(ui->actionGames);
    ui->toolButtonPlaces->setDefaultAction(ui->actionPlaces);
    ui->toolButtonPlayers->setDefaultAction(ui->actionPlayers);

    ui->treeViewPlayers->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewGames->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewPlaces->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewDrinks->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui->actionInformation, &QAction::triggered,
            ui->actionPlayerInformation, &QAction::trigger);
    connect(ui->actionInformation, &QAction::triggered,
            ui->actionDrinkInformation, &QAction::trigger);

    m_playerListModel = new PlayersListModel(this);
    QSortFilterProxyModel *sortModel = new QSortFilterProxyModel(this);
    sortModel->setSourceModel(m_playerListModel);
    ui->treeViewPlayers->setModel(sortModel);
    ui->treeViewPlayers->addAction(ui->actionPlayerInformation);
    connect(ui->treeViewPlayers, &QTreeView::doubleClicked,
            this, &MainWindow::on_actionPlayerInformation_triggered);

    GameListModel *modelGames = new GameListModel(this);
    sortModel = new QSortFilterProxyModel(this);
    sortModel->setSourceModel(modelGames);
    ui->treeViewGames->setModel(sortModel);
    ui->treeViewGames->setSortingEnabled(true);

    DrinksListModel *modelDrinks = new DrinksListModel(this);
    sortModel = new QSortFilterProxyModel(this);
    sortModel->setSourceModel(modelDrinks);
    ui->treeViewDrinks->setModel(sortModel);
    ui->treeViewDrinks->addAction(ui->actionDrinkInformation);
    connect(ui->treeViewDrinks, &QTreeView::doubleClicked,
            this, &MainWindow::on_actionDrinkInformation_triggered);

    QTimer::singleShot(0, this, SLOT(restoreWindowState()));
    QTimer::singleShot(0, this, SLOT(show()));
    MenuBar::instance()->addAction(tr("&File"), ui->actionNew_Game, this);
    MenuBar::instance()->addAction(tr("&File"), ui->actionInformation, this);
    MenuBar::instance()->addAction(tr("&View"), ui->actionPlayers, this);
    MenuBar::instance()->addAction(tr("&View"), ui->actionGames, this);
    MenuBar::instance()->addAction(tr("&View"), ui->actionPlaces, this);
    MenuBar::instance()->addAction(tr("&View"), ui->actionDrinks, this);
    MenuBar::instance()->menu(tr("&View"))->addSeparator();
}

MainWindow::~MainWindow()
{
    saveWindowState();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveWindowState();
    QMainWindow::closeEvent(event);
}

void MainWindow::saveWindowState()
{
    QSettings settings;
    settings.setValue("mainwindow/geometry", saveGeometry());
    settings.setValue("mainwindow/windowState", saveState());
    settings.setValue("mainwindow/treeviewplayers/state", ui->treeViewPlayers->header()->saveState());
    settings.setValue("mainwindow/treeviewgames/state", ui->treeViewGames->header()->saveState());
    settings.setValue("mainwindow/treeviewdrinks/state", ui->treeViewDrinks->header()->saveState());
    settings.setValue("mainwindow/splitter", ui->splitter->saveState());
}

void MainWindow::restoreWindowState()
{
    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/windowState").toByteArray());
    ui->treeViewPlayers->header()->restoreState(settings.value("mainwindow/treeviewplayers/state").toByteArray());
    ui->treeViewGames->header()->restoreState(settings.value("mainwindow/treeviewgames/state").toByteArray());
    ui->treeViewDrinks->header()->restoreState(settings.value("mainwindow/treeviewdrinks/state").toByteArray());
    ui->splitter->restoreState(settings.value("mainwindow/splitter").toByteArray());
}

void MainWindow::on_actionPlayers_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pagePlayers);
}

void MainWindow::on_actionGames_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pageGames);
}

void MainWindow::on_actionPlaces_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pagePlaces);
}

void MainWindow::on_actionDrinks_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pageDrinks);
}

void MainWindow::on_actionPlayerInformation_triggered()
{
    if(ui->stackedWidget->currentWidget() != ui->pagePlayers)
        return;

    QModelIndexList list = ui->treeViewPlayers->selectionModel()->selectedIndexes();
    if(list.isEmpty())
        return;

    PlayerInformationDialog dialog;
    dialog.setPlayerFromModel(m_playerListModel,
                              list.first().row());
    dialog.exec();
}

void MainWindow::on_actionDrinkInformation_triggered()
{
    if(ui->stackedWidget->currentWidget() != ui->pageDrinks)
        return;

    QSharedPointer<Drink> drink = Tools::selectedObjectFrom<Drink>(ui->treeViewDrinks);
    if(!drink)
        return;

    DrinkInformationDialog dialog;
    dialog.setDrink(drink);
    dialog.exec();
}

void MainWindow::on_treeViewGames_doubleClicked(const QModelIndex &)
{
    if(ui->stackedWidget->currentWidget() != ui->pageGames)
        return;

    QSharedPointer<Game> game = Tools::selectedObjectFrom<Game>(ui->treeViewGames);
    if(!game)
        return;

    GameWindow *window = new GameWindow;
    window->setGame(game);
    window->show();
}

void MainWindow::on_actionNew_Game_triggered()
{
    GameWindow *window = new GameWindow;
    window->show();
}
