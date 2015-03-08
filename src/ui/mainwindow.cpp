#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogs/playerinformationdialog.h"
#include "dialogs/drinkinformationdialog.h"
#include "dialogs/placeinformationdialog.h"
#include "game/gamewindow.h"
#include "chooselibrarywidget.h"
#include "ui/league/addleaguedialog.h"
#include "ui/dialogs/addphotosdialog.h"

#include <ui/delegates/gamesdelegate.h>
#include <ui/delegates/playersdelegate.h>

#include <library.h>
#include <ui/model/playerslistmodel.h>
#include <ui/model/gamelistmodel.h>
#include <ui/model/drinkslistmodel.h>
#include <ui/model/placeslistmodel.h>
#include <misc/tools.h>
#include <data/game.h>
#include <misc/updater/updater.h>

#include <QApplication>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QSettings>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_playerListModel(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actionStart);
    actionGroup->addAction(ui->actionDrinks);
    actionGroup->addAction(ui->actionGames);
    actionGroup->addAction(ui->actionPlaces);
    actionGroup->addAction(ui->actionPlayers);
    actionGroup->addAction(ui->actionPhotos);
    actionGroup->setExclusive(true);
    ui->actionStart->setChecked(true);

    ui->toolButtonStart->setDefaultAction(ui->actionStart);
    ui->toolButtonDrinks->setDefaultAction(ui->actionDrinks);
    ui->toolButtonGames->setDefaultAction(ui->actionGames);
    ui->toolButtonPlaces->setDefaultAction(ui->actionPlaces);
    ui->toolButtonPlayers->setDefaultAction(ui->actionPlayers);
    ui->toolButtonPhotos->setDefaultAction(ui->actionPhotos);

    ui->listViewPlayers->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listViewGames->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewPlaces->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewDrinks->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui->actionInformation, &QAction::triggered,
            ui->actionPlayerInformation, &QAction::trigger);
    connect(ui->actionInformation, &QAction::triggered,
            ui->actionDrinkInformation, &QAction::trigger);
    connect(ui->actionClose, &QAction::triggered, this, &QWidget::close);

    ui->listViewPlayers->addAction(ui->actionPlayerInformation);
    connect(ui->listViewPlayers, &QListView::doubleClicked,
            this, &MainWindow::on_actionPlayerInformation_triggered);

    ui->treeViewDrinks->addAction(ui->actionDrinkInformation);
    connect(ui->treeViewDrinks, &QTreeView::doubleClicked,
            this, &MainWindow::on_actionDrinkInformation_triggered);

    ui->actionCheck_for_updates->setMenuRole(QAction::ApplicationSpecificRole);

    ui->pageStart->init(this);

    setWindowFilePath(Library::instance()->fileName());

    QTimer::singleShot(0, this, SLOT(restoreWindowState()));
    QTimer::singleShot(0, this, SLOT(show()));

    ui->stackedWidget->setCurrentIndex(0);
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
    settings.setValue("mainwindow/treeviewplaces/state", ui->treeViewPlaces->header()->saveState());
    //settings.setValue("mainwindow/listViewPlayers/state", ui->listViewPlayers->header()->saveState());
    //settings.setValue("mainwindow/treeviewgames/state", ui->listViewGames->header()->saveState());
    settings.setValue("mainwindow/treeviewdrinks/state", ui->treeViewDrinks->header()->saveState());
}

void MainWindow::restoreWindowState()
{
    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/windowState").toByteArray());
    ui->treeViewPlaces->header()->restoreState(settings.value("mainwindow/treeviewplaces/state").toByteArray());
    //ui->listViewPlayers->header()->restoreState(settings.value("mainwindow/listViewPlayers/state").toByteArray());
   // ui->listViewGames->header()->restoreState(settings.value("mainwindow/treeviewgames/state").toByteArray());
    ui->treeViewDrinks->header()->restoreState(settings.value("mainwindow/treeviewdrinks/state").toByteArray());
}

void MainWindow::on_actionStart_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pageStart);
}

void MainWindow::on_actionPlayers_triggered()
{
    if(!ui->listViewPlayers->model()) {
        m_playerListModel = new PlayersSortFilterModel(this);
        m_playerListModel->setSortRole(PlayersSortFilterModel::Games);
        m_playerListModel->sort(0, Qt::DescendingOrder);
        ui->listViewPlayers->setModel(m_playerListModel);
        ui->listViewPlayers->setItemDelegate(new PlayersDelegate(ui->listViewPlayers, this));
    }

    ui->stackedWidget->setCurrentWidget(ui->pagePlayers);
}

void MainWindow::on_actionGames_triggered()
{
    if(!ui->listViewGames->model()) {
        GameSortFilterModel *modelGames = new GameSortFilterModel(new GameListModel(this), this);
        modelGames->setSortRole(GameSortFilterModel::Date);
        modelGames->sort(0, Qt::DescendingOrder);
        ui->listViewGames->setModel(modelGames);
        ui->listViewGames->setItemDelegate(new GamesDelegate(ui->listViewGames, this));
    }

    ui->stackedWidget->setCurrentWidget(ui->pageGames);
}

void MainWindow::on_actionPlaces_triggered()
{
    if(!ui->treeViewPlaces->model()) {
        QpSortFilterProxyObjectModel<Place> *model = new QpSortFilterProxyObjectModel<Place>(new PlacesListModel(this), this);
        ui->treeViewPlaces->setModel(model);

        QSettings settings;
        if(!settings.contains("mainwindow/treeviewplaces/state")) {
            ui->treeViewPlaces->sortByColumn(PlacesListModel::GameCountColumn, Qt::DescendingOrder);
            QHeaderView *h = ui->treeViewPlaces->header();
            h->resizeSection(PlacesListModel::CityEmblemColumn, 22);
            h->resizeSection(PlacesListModel::PostalCodeColumn, 65);
            h->resizeSection(PlacesListModel::CityColumn, 80);
            h->resizeSection(PlacesListModel::StreetColumn, 160);
            h->resizeSection(PlacesListModel::HouseNumberColumn, 40);
            h->resizeSection(PlacesListModel::CommentColumn, 100);
            h->resizeSection(PlacesListModel::PlayersColumn, 140);
        }
    }

    ui->stackedWidget->setCurrentWidget(ui->pagePlaces);
}

void MainWindow::on_actionDrinks_triggered()
{
    if(!ui->treeViewDrinks->model()) {
        QpSortFilterProxyObjectModel<Drink> *modelDrinks = new QpSortFilterProxyObjectModel<Drink>(new DrinksListModel(this), this);
        ui->treeViewDrinks->setModel(modelDrinks);
    }

    ui->stackedWidget->setCurrentWidget(ui->pageDrinks);
}

void MainWindow::on_actionPlayerInformation_triggered()
{
    if(ui->stackedWidget->currentWidget() != ui->pagePlayers)
        return;

    QModelIndexList list = ui->listViewPlayers->selectionModel()->selectedIndexes();
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

    QSharedPointer<Game> game = Tools::selectedObjectFrom<Game>(ui->listViewGames);
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

void MainWindow::on_actionOpen_library_triggered()
{
    close();

    ChooseLibraryWidget *widget = new ChooseLibraryWidget;
    widget->show();
}

void MainWindow::on_actionNew_League_triggered()
{
    AddLeagueDialog dlg(this);
    dlg.exec();
}

void MainWindow::on_actionAdd_Photos_triggered()
{
    QSettings settings;
    QString openFolder = settings.value("mainwindow/addphotodialog/defaultLocation", QStandardPaths::writableLocation(QStandardPaths::HomeLocation)).toString();

    QStringList list = QFileDialog::getOpenFileNames(
                    this,
                    "Select one or more photos to add",
                    openFolder,
                    "Images (*.png *.xpm *.jpg)");

    if(list.isEmpty())
        return;

    settings.setValue("mainwindow/addphotodialog/defaultLocation", QFileInfo(list.first()).absolutePath());

    AddPhotosDialog dlg(this);
    dlg.setFilesToAdd(list);
    int returnValue = dlg.exec();
    if(returnValue == QDialog::Accepted) {
        emit photosAdded();
    }
}

void MainWindow::on_actionPhotos_triggered()
{
    ui->pagePhotos->setMainWindow(this);
    ui->pagePhotos->setGames(Qp::readAll<Game>());
    ui->stackedWidget->setCurrentWidget(ui->pagePhotos);
}

void MainWindow::on_actionCheck_for_updates_triggered()
{
    Updater::instanceForPlatform()->checkForUpdatesInBackground();
}

void MainWindow::on_actionNew_Player_triggered()
{
    QSharedPointer<Player> newPlayer = Qp::create<Player>();
    newPlayer->setName("NewPlayer");

    PlayerInformationDialog dialog;
    dialog.setPlayer(newPlayer);
    dialog.exec();
}

void MainWindow::on_actionNew_Place_triggered()
{
    PlaceInformationDialog dialog;
    dialog.exec();
}

void MainWindow::on_actionNew_Drink_triggered()
{
    DrinkInformationDialog dialog;
    dialog.exec();
}
