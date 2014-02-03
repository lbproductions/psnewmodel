#include "gamewindow.h"
#include "ui_gamewindow.h"

#include "overviewplayerheaderview.h"
#include "overviewhorizontalheaderview.h"
#include "overviewdelegate.h"
#include "newrounddialog.h"
#include "schmeissereidialog.h"
#include "statisticswidget.h"
#include "drinkswidget.h"
#include "commentwidget.h"
#include "stopgamewidget.h"
#include "settingswidget.h"
#include "gamestatswidget.h"
#include "resumewidget.h"
#include "dialogcontroller.h"

#include <ui/widgets/playerslistwidget.h>
#include <ui/widgets/menubar.h>
#include <ui/widgets/popupwidget.h>
#include <data/game.h>
#include <data/place.h>
#include <model/gameoverviewmodel.h>
#include <model/gameinformationmodel.h>
#include <data/player.h>
#include <misc/tools.h>
#include <misc/settings.h>

#include <QPersistence.h>
#include <QScrollBar>
#include <QWheelEvent>
#include <QPushButton>
#include <QModelIndex>
#include <QPropertyAnimation>

#include <QMessageBox>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);
    QPalette darkPalette = palette();
    darkPalette.setColor(QPalette::Window, QColor(71,71,71));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(55,55,55));
    darkPalette.setColor(QPalette::AlternateBase, QColor(49,49,49));
    darkPalette.setColor(QPalette::ToolTipBase, darkPalette.color(QPalette::Base));
    darkPalette.setColor(QPalette::ToolTipText, darkPalette.color(QPalette::WindowText));
    darkPalette.setColor(QPalette::Text, QColor(228,228,228));
    darkPalette.setColor(QPalette::Button, QColor(61,61,61));
    darkPalette.setColor(QPalette::ButtonText, QColor(226,226,226));
    darkPalette.setColor(QPalette::BrightText, QColor(43,43,43));
    darkPalette.setColor(QPalette::Highlight, QColor(108,108,108));
    darkPalette.setColor(QPalette::HighlightedText, QColor(187,187,187));
    setPalette(darkPalette);

    ui->listWidgetPlayers->setDragDropMode(QAbstractItemView::InternalMove);

    ui->tableViewInformation->hide();
    ui->tableViewOverview->hide();

    ui->comboBoxSite->addPlaces(Qp::readAll<Place>());
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->listWidgetPlayers->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidgetPlayers->setPalette(darkPalette);
    ui->graphAxis->setFixedWidth(ui->listWidgetPlayers->width());

    m_gameOverViewModel = new GameOverviewModel(this);
    ui->tableViewOverview->setPalette(darkPalette);
    ui->tableViewOverview->setModel(m_gameOverViewModel);
    connect(ui->tableViewOverview, &QTableView::doubleClicked,
            this, &GameWindow::onTableViewOverviewDoubleClicked);
    OverviewDelegate *delegate = new OverviewDelegate(this);
    delegate->setGameModel(m_gameOverViewModel);
    ui->tableViewOverview->setItemDelegate(delegate);
    OverviewHorizontalHeaderView *horizontalHeaderView = new OverviewHorizontalHeaderView(Qt::Horizontal, this);
    ui->tableViewOverview->setHorizontalHeader(horizontalHeaderView);

    m_informationModel = new GameInformationModel(this);
    ui->tableViewInformation->setItemDelegate(delegate);
    ui->tableViewInformation->setPalette(darkPalette);
    ui->tableViewInformation->setModel(m_informationModel);
    m_verticalHeaderView = new OverviewPlayerHeaderView(Qt::Vertical, this);
    m_verticalHeaderView->setGameModel(m_informationModel);
    ui->tableViewInformation->setVerticalHeader(m_verticalHeaderView);
    OverviewHorizontalHeaderView *horizontalHeaderView2 = new OverviewHorizontalHeaderView(Qt::Horizontal, this);
    ui->tableViewInformation->setHorizontalHeader(horizontalHeaderView2);

    ui->graphWidget->setPalette(darkPalette);
    ui->graphAxis->setPalette(darkPalette);
    ui->graphAxis->setGraph(ui->graphWidget);
    ui->graphWidget->stackUnder(ui->graphAxis);

    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actionAdd_drinks);
    actionGroup->addAction(ui->actionAdd_Hochzeit);
    actionGroup->addAction(ui->actionAdd_round);
    actionGroup->addAction(ui->actionAdd_schmeisserei);
    actionGroup->addAction(ui->actionAdd_Solo);
    actionGroup->addAction(ui->actionAdd_Trumpfabgabe);
    actionGroup->addAction(ui->actionStop_Game);
    actionGroup->setExclusive(true);
    m_informationModel->setHeaderAction(GameOverviewModel::NormalRoundRow, ui->actionAdd_round);
    m_informationModel->setHeaderAction(GameOverviewModel::SchmeissereienRow, ui->actionAdd_schmeisserei);
    m_informationModel->setHeaderAction(GameOverviewModel::DrinksRow, ui->actionAdd_drinks);
    m_informationModel->setHeaderAction(GameOverviewModel::HochzeitenRow, ui->actionAdd_Hochzeit);
    m_informationModel->setHeaderAction(GameOverviewModel::TrumpfabgabenRow, ui->actionAdd_Trumpfabgabe);
    m_informationModel->setHeaderAction(GameOverviewModel::SoliRow, ui->actionAdd_Solo);

    m_actionsController = new DialogController(this);
    connect(m_actionsController, &DialogController::dialogClosed, [this, actionGroup] {
        actionGroup->checkedAction()->setChecked(false);
        ui->tableViewInformation->horizontalHeader()->repaint();
    });

    m_resumeWidget = new ResumeWidget(this);
    m_resumeWidget->setVisible(false);

//    QTimer *lengthTimer = new QTimer(this);
//    connect(lengthTimer, &QTimer::timeout,
//            this, &GameWindow::updateTimes);
//    lengthTimer->start(1000);

    MenuBar::instance()->addAction(tr("&Game"), ui->actionPlayPause, this);
    MenuBar::instance()->addAction(tr("&Game"), ui->actionStop_Game, this);
    MenuBar::instance()->menu(tr("&Game"))->addSeparator();
    MenuBar::instance()->addAction(tr("&Game"), ui->actionAdd_round, this);
    MenuBar::instance()->addAction(tr("&Game"), ui->actionAdd_Hochzeit, this);
    MenuBar::instance()->addAction(tr("&Game"), ui->actionAdd_Solo, this);
    MenuBar::instance()->addAction(tr("&Game"), ui->actionAdd_Trumpfabgabe, this);
    MenuBar::instance()->addAction(tr("&Game"), ui->actionAdd_schmeisserei, this);
    MenuBar::instance()->addAction(tr("&Game"), ui->actionAdd_drinks, this);
    MenuBar::instance()->addAction(tr("&Game"), ui->actionStats, this);
    MenuBar::instance()->addAction(tr(""), ui->actionSettings, this);

    enableActionsBasedOnState();

    void (GameSettings::*showExtraRowsChangedSignal)(bool) = &GameSettings::showExtraRowsChanged;
    connect(&GameSettings::instance(), showExtraRowsChangedSignal, this, &GameWindow::updateSizes);
}

GameWindow::~GameWindow()
{
    if(m_game) {
        m_game->pause();
        m_game->save();
    }
    delete ui;
}

void GameWindow::setGame(const QSharedPointer<Game> &game)
{
    ui->widgetCreateGame->hide();
    ui->tableViewOverview->show();
    ui->tableViewInformation->show();

    m_game = game;

    m_gameOverViewModel->setGame(game);
    m_informationModel->setGame(game);
    ui->graphWidget->setGame(game);
    ui->gameLengthWidget->setGame(game);

    if(m_game->state() == Game::Paused) {
        m_resumeWidget->resize(width(), height());
        m_resumeWidget->setVisible(true);
        connect(m_resumeWidget, &ResumeWidget::widgetClicked,
                this, &GameWindow::on_actionPlayPause_triggered);
    }

    connect(m_game.data(), &Game::newRoundStarted, this, &GameWindow::onNewRoundStarted);
    connect(m_game.data(), &Game::stateChanged, this, &GameWindow::enableActionsBasedOnState);

    enableActionsBasedOnState();
    updateSizes();
}

void GameWindow::updateSizes()
{
    int overviewHeight = ui->tableViewOverview->horizontalHeader()->height() +
            (m_gameOverViewModel->rowCount()) * ui->tableViewOverview->rowHeight(0);
    ui->tableViewOverview->setFixedHeight(overviewHeight);
    ui->tableViewInformation->setFixedHeight(overviewHeight);
    m_actionsController->setDialogOffsetTop(1 + ui->tableViewInformation->horizontalHeader()->height());
    m_actionsController->setDialogHeight(overviewHeight - m_actionsController->dialogOffsetTop() - 1);
    ui->tableViewInformation->setFixedWidth(ui->tableViewInformation->verticalHeader()->width() + 39);
    ui->graphAxis->setFixedWidth(ui->tableViewInformation->verticalHeader()->width() + 40);
}

void GameWindow::mousePressEvent(QMouseEvent *e)
{
    m_actionsController->closeDialogOnMousePress(e->pos());
    QMainWindow::mousePressEvent(e);
}

void GameWindow::resizeEvent(QResizeEvent *)
{
    m_resumeWidget->resize(width(), height());
}

void GameWindow::enableActionsBasedOnState()
{
    ui->actionPlayPause->setEnabled(false);
    ui->actionAdd_round->setEnabled(false);
    ui->actionAdd_schmeisserei->setEnabled(false);
    ui->actionStop_Game->setEnabled(false);

    if(!m_game)
        return;

    Game::State state = m_game->state();
    if(state == Game::Running) {
        ui->actionAdd_round->setEnabled(true);
        ui->actionAdd_schmeisserei->setEnabled(true);
        ui->actionPlayPause->setEnabled(true);
        ui->actionPlayPause->setText(tr("Pause"));
        ui->actionStop_Game->setEnabled(true);

        m_resumeWidget->setVisible(false);
    }
    else if(state == Game::Paused) {
        ui->actionPlayPause->setEnabled(true);
        ui->actionStop_Game->setEnabled(true);
        ui->actionPlayPause->setText(tr("Play"));
        m_resumeWidget->setVisible(true);
    }
    else {
        ui->actionPlayPause->setText(tr("Pause"));
    }
}

/**********************************************************
 * State
 */
void GameWindow::on_actionPlayPause_triggered()
{
    m_game->togglePlayPause();
    m_game->save();
    enableActionsBasedOnState();
}

void GameWindow::on_actionStop_Game_triggered()
{
    StopGameWidget* widget = new StopGameWidget(this);
    widget->setGame(m_game);
    m_actionsController->showDialog(widget);
}

void GameWindow::onNewRoundStarted()
{
    ui->graphWidget->updateGraphs();

    if(GameSettings::instance().gamePercentageWarning()) {
        int oldPercentage = (m_game->rounds().size()-2) * 100 / m_game->totalRoundCount();
        if(oldPercentage < 50 && m_game->completedPercentage() >= 50) {
            QMessageBox box(this);
            box.setPalette(Tools::darkPalette(&box));
            box.setText(tr("This game is to 50% complete!"));
            box.exec();
        }
    }
}

/**********************************************************
 * Dialogs and popups
 */
void GameWindow::on_actionAdd_round_triggered()
{
    NewRoundDialog *dialog = new NewRoundDialog(this);
    dialog->setRound(m_game->currentRound());
    m_actionsController->showDialog(dialog);
}

void GameWindow::on_actionAdd_Hochzeit_triggered()
{
    NewRoundDialog *dialog = new NewRoundDialog(this);
    dialog->setRound(m_game->currentRound());
    dialog->setCurrentPage(1);
    m_actionsController->showDialog(dialog);
}

void GameWindow::on_actionAdd_Solo_triggered()
{
    NewRoundDialog *dialog = new NewRoundDialog(this);
    dialog->setRound(m_game->currentRound());
    dialog->setCurrentPage(2);
    m_actionsController->showDialog(dialog);
}

void GameWindow::on_actionAdd_Trumpfabgabe_triggered()
{
    NewRoundDialog *dialog = new NewRoundDialog(this);
    dialog->setRound(m_game->currentRound());
    dialog->setCurrentPage(3);
    m_actionsController->showDialog(dialog);
}

void GameWindow::on_actionAdd_schmeisserei_triggered()
{
    SchmeissereiDialog *dialog = new SchmeissereiDialog(this);
    dialog->setGame(m_game);
    m_actionsController->showDialog(dialog);
}

void GameWindow::on_actionAdd_drinks_triggered()
{
    DrinksWidget *drinksWidget = new DrinksWidget(ui->centralwidget);
    drinksWidget->setRound(m_game->currentRound());
    m_actionsController->showDialog(drinksWidget);
}

void GameWindow::on_toolButtonSetComment_clicked()
{
    CommentWidget* commentWidget = new CommentWidget(this);
    commentWidget->setGame(m_game);
    m_actionsController->showDialog(commentWidget);
}

void GameWindow::on_actionStats_triggered()
{
    GameStatsWidget* widget = new GameStatsWidget(this);
    widget->setGames(QList<QSharedPointer<Game> >() << m_game);
    widget->show();
}

void GameWindow::on_actionSettings_triggered()
{
    PopupWidget *popup = new PopupWidget(this);

    SettingsWidget* setWidget = new SettingsWidget(this);

    popup->setWidget(setWidget);
    popup->setMinimumWidth(400);
    popup->setMinimumHeight(500);
    popup->show();
}

void GameWindow::onTableViewOverviewDoubleClicked(const QModelIndex& index)
{
    int roundIndex = index.column();

    if(roundIndex < 0 || roundIndex >= m_game->rounds().size())
        return;

    QSharedPointer<Round> round = m_game->rounds().at(roundIndex);
    NewRoundDialog* dlg = new NewRoundDialog(this);
    dlg->setRound(round, NewRoundDialog::EditRound);
    m_actionsController->showDialog(dlg);
}


/**********************************************************
 * PRE-Game
 */
void GameWindow::on_pushButtonAddPlayers_clicked()
{
    PopupWidget *popup = new PopupWidget(this);

    PlayersListWidget *list = new PlayersListWidget(popup);
    list->setPalette(palette());
    list->setAttribute(Qt::WA_MacShowFocusRect, false);
    list->add(Qp::readAll<Player>());

    foreach(QSharedPointer<Player> p, ui->listWidgetPlayers->items())
        list->remove(p);

    connect(list, &PlayersListWidget::playerActivated,
            this, &GameWindow::addPlayerToGame);

    popup->setWidget(list);
    popup->setMinimumWidth(400);
    popup->setMinimumHeight(500);
    popup->setArrowPosition(PopupWidget::Left);
    popup->anchorTo(ui->pushButtonAddPlayers);
    popup->show();
}

void GameWindow::addPlayerToGame(QSharedPointer<Player> player)
{
    PlayersListWidget *list = static_cast<PlayersListWidget *>(sender());
    list->remove(player);
    ui->listWidgetPlayers->add(player);
    if(ui->listWidgetPlayers->count() >= 4)
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void GameWindow::on_buttonBox_accepted()
{
    QSharedPointer<Game> game = Qp::create<Game>();
    game->setMitPflichtSolo(ui->checkBoxMitPflichtSoli->isChecked());
    game->setSite(ui->comboBoxSite->currentPlace());

    foreach(QSharedPointer<Player> p, ui->listWidgetPlayers->items()) {
        game->addPlayer(p);
    }

    game->setType(Game::Doppelkopf);
    game->startNextRound();
    game->setState(Game::Running);
    setGame(game);
}
