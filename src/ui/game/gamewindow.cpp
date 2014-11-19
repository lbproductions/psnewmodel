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
#include <ui/dialogs/drinkinformationdialog.h>
#include <data/game.h>
#include <data/place.h>
#include <ui/model/gameoverviewmodel.h>
#include <ui/model/gameinformationmodel.h>
#include <data/player.h>
#include <misc/tools.h>
#include <misc/settings.h>
#include <misc/updater/updater.h>

#include <QPersistence.h>
#include <QScrollBar>
#include <QWheelEvent>
#include <QPushButton>
#include <QModelIndex>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QEvent>
#include <QMessageBox>
#include <QSettings>

#include <data/parsecontroller.h>

bool sortPlaces(const QSharedPointer<Place> &p1, const QSharedPointer<Place> &p2);

QMultiHash<QSharedPointer<Game>, GameWindow *> GameWindow::s_gameWindows;

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow),
    m_minimumColumnWidth(20)
{   
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

    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);

    ui->listWidgetPlayers->setDragDropMode(QAbstractItemView::InternalMove);

    ui->tableViewInformation->hide();
    ui->tableViewOverview->hide();

    QList<QSharedPointer<Place>> places = Qp::readAll<Place>();
    qSort(places.begin(), places.end(), sortPlaces);
    ui->comboBoxSite->addPlaces(places);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->listWidgetPlayers->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidgetPlayers->setPalette(darkPalette);
    ui->graphAxis->setFixedWidth(ui->listWidgetPlayers->width());
    ui->buttonBox->setFixedWidth(ui->comboBoxSite->width() + ui->labelSite->width() + 200);

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
    actionGroup->addAction(ui->actionSet_comment);
    actionGroup->addAction(ui->actionStats);
    actionGroup->setExclusive(true);

    m_dialogController = new DialogController(this);

    connect(m_dialogController, &DialogController::dialogClosed,
            this, &GameWindow::onDialogClosed);

    m_resumeWidget = new ResumeWidget(this);
    m_resumeWidget->setVisible(false);

    QTimer *updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, [=] {
        ParseController::instance()->update();
    });
    updateTimer->start(1000);
    //    QTimer *lengthTimer = new QTimer(this);
    //    connect(lengthTimer, &QTimer::timeout,
    //            this, &GameWindow::updateTimes);
    //    lengthTimer->start(1000);

    enableActionsBasedOnState();

    void (GameSettings::*showExtraRowsChangedSignal)(bool) = &GameSettings::showExtraRowsChanged;
    connect(&GameSettings::instance(), showExtraRowsChangedSignal, this, &GameWindow::updateSizes);

    connect(ui->actionClose_window, &QAction::triggered, this, &QWidget::close);

    installEventFilter(this);
    ui->tableViewInformation->viewport()->installEventFilter(this);
    ui->tableViewOverview->viewport()->installEventFilter(this);

    restoreWindowState();
}

GameWindow::~GameWindow()
{
    if(m_game) {
        s_gameWindows.remove(m_game, this);
        if(s_gameWindows.values(m_game).isEmpty()) {
            m_game->pause();
            m_game->save();
        }
    }
    saveWindowState();
    delete ui;
}

void GameWindow::saveWindowState()
{
    QSettings settings;
    settings.setValue("gamewindow/geometry", saveGeometry());
    settings.setValue("gamewindow/windowState", saveState());
    settings.setValue("gamewindow/fontsize", m_gameOverViewModel->fontSize());
}

void GameWindow::restoreWindowState()
{
    QSettings settings;
    restoreGeometry(settings.value("gamewindow/geometry").toByteArray());
    restoreState(settings.value("gamewindow/windowState").toByteArray());
    m_gameOverViewModel->setFontSize(settings.value("gamewindow/fontsize", 16).toInt());
    m_informationModel->setFontSize(settings.value("gamewindow/fontsize", 16).toInt());
}

void GameWindow::setGame(const QSharedPointer<Game> &game)
{
    ui->widgetCreateGame->hide();
    ui->tableViewOverview->show();
    ui->tableViewInformation->show();

    m_game = game;

    if(m_game->state() == Game::Running && s_gameWindows.values(m_game).isEmpty())
        m_game->setState(Game::Paused);

    s_gameWindows.insert(m_game, this);

    m_gameOverViewModel->setGame(game);
    m_informationModel->setGame(game);
    ui->graphWidget->setGame(game);
    ui->gameLengthWidget->setGame(game);

    if(m_game->state() != Game::Finished) {
        m_resumeWidget->resize(width(), height());
        m_resumeWidget->setVisible(true);
        connect(m_resumeWidget, &ResumeWidget::widgetClicked,
                this, &GameWindow::on_actionPlayPause_triggered);

        connect(m_game.data(), &Game::newRoundStarted, this, &GameWindow::onNewRoundStarted);
        connect(ParseController::instance(), &ParseController::updateFinished, this, &GameWindow::onNewRoundStarted);
        connect(m_game.data(), &Game::stateChanged, this, &GameWindow::enableActionsBasedOnState);
    }

    enableActionsBasedOnState();
    updateSizes();
}

bool GameWindow::eventFilter(QObject *obj, QEvent *event)
{
    static ulong mouseClickLastTime = -1;

    if(event->type() == QEvent::MouseButtonRelease) {
        // This is shit: QTBUG-25831
        // TODO: Change when Qt 5.3 is released...

        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        bool isDoubleClick = (me->timestamp() - mouseClickLastTime )
                < static_cast<ulong>(QApplication::doubleClickInterval());
        mouseClickLastTime = me->timestamp();

        if(!isDoubleClick)
            m_dialogController->closeDialogOnMousePress(me->pos());
    }
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if(ke->key() == Qt::Key_Escape) {
            m_dialogController->closeDialog();
        }
    }

    return QObject::eventFilter(obj, event);
}

void GameWindow::updateSizes()
{
    ui->tableViewOverview->verticalHeader()->setDefaultSectionSize(m_gameOverViewModel->fontSize() + 15);
    ui->tableViewInformation->verticalHeader()->setDefaultSectionSize(m_gameOverViewModel->fontSize() + 15);
    int overviewHeight = ui->tableViewOverview->horizontalHeader()->height() +
            (m_gameOverViewModel->rowCount()) * ui->tableViewOverview->rowHeight(0);
    ui->tableViewOverview->setFixedHeight(overviewHeight);
    ui->tableViewInformation->setFixedHeight(overviewHeight);
    m_dialogController->setDialogOffsetTop(1 + ui->tableViewInformation->horizontalHeader()->height());
    m_dialogController->setDialogOffsetLeft(29);
    m_dialogController->setDialogHeight(overviewHeight - m_dialogController->dialogOffsetTop() - 1);
    ui->tableViewInformation->setFixedWidth(ui->tableViewInformation->verticalHeader()->width() + 39);
    ui->graphAxis->setFixedWidth(ui->tableViewInformation->verticalHeader()->width() + 40);
    resize(size() + QSize(1,0));
    resize(size() - QSize(1,0));
}

void GameWindow::resizeEvent(QResizeEvent *)
{
    m_resumeWidget->resize(width(), height());

    if(!m_game)
        return;

    int w = width();
    int roundCount = m_game->totalRoundCount();

    w -= ui->tableViewInformation->width();
    w /= roundCount + 1;
    w = qMax(m_minimumColumnWidth, w);

    ui->tableViewOverview->horizontalHeader()->setDefaultSectionSize(w);
    ui->graphWidget->setColumnWidth(w);
}

void GameWindow::onDialogClosed()
{
    ui->graphWidget->updateGraphs();
    m_gameOverViewModel->updateViews();
    m_informationModel->updateViews();

    if(!ui->actionAdd_drinks->actionGroup()->checkedAction())
        return;

    ui->actionAdd_drinks->actionGroup()->checkedAction()->setChecked(false);
    ui->tableViewInformation->horizontalHeader()->repaint();
}

void GameWindow::enableActionsBasedOnState()
{
    ui->actionPlayPause->setEnabled(false);
    ui->actionAdd_round->setEnabled(false);
    ui->actionAdd_schmeisserei->setEnabled(false);
    ui->actionStop_Game->setEnabled(false);
    ui->actionAdd_drinks->setEnabled(false);
    ui->actionAdd_Hochzeit->setEnabled(false);
    ui->actionAdd_Solo->setEnabled(false);
    ui->actionAdd_Trumpfabgabe->setEnabled(false);

    m_informationModel->removeHeaderAction(GameOverviewModel::NormalRoundRow, ui->actionAdd_round);
    m_informationModel->removeHeaderAction(GameOverviewModel::SchmeissereienRow, ui->actionAdd_schmeisserei);
    m_informationModel->removeHeaderAction(GameOverviewModel::DrinksRow, ui->actionAdd_drinks);
    m_informationModel->removeHeaderAction(GameOverviewModel::HochzeitenRow, ui->actionAdd_Hochzeit);
    m_informationModel->removeHeaderAction(GameOverviewModel::TrumpfabgabenRow, ui->actionAdd_Trumpfabgabe);
    m_informationModel->removeHeaderAction(GameOverviewModel::SoliRow, ui->actionAdd_Solo);

    if(!m_game)
        return;

    Game::State state = m_game->state();
    if(state == Game::Running) {
        ui->actionAdd_round->setEnabled(true);
        ui->actionAdd_schmeisserei->setEnabled(true);
        ui->actionPlayPause->setEnabled(true);
        ui->actionPlayPause->setText(tr("Pause"));
        ui->actionAdd_drinks->setEnabled(true);
        ui->actionAdd_Hochzeit->setEnabled(true);
        ui->actionAdd_Solo->setEnabled(true);
        ui->actionAdd_Trumpfabgabe->setEnabled(true);

        if(m_game->rounds().size() > 1)
            ui->actionStop_Game->setEnabled(true);

        m_resumeWidget->setVisible(false);

        m_informationModel->setHeaderAction(GameOverviewModel::NormalRoundRow, ui->actionAdd_round);
        m_informationModel->setHeaderAction(GameOverviewModel::SchmeissereienRow, ui->actionAdd_schmeisserei);
        m_informationModel->setHeaderAction(GameOverviewModel::DrinksRow, ui->actionAdd_drinks);
        m_informationModel->setHeaderAction(GameOverviewModel::HochzeitenRow, ui->actionAdd_Hochzeit);
        m_informationModel->setHeaderAction(GameOverviewModel::TrumpfabgabenRow, ui->actionAdd_Trumpfabgabe);
        m_informationModel->setHeaderAction(GameOverviewModel::SoliRow, ui->actionAdd_Solo);
    }
    else if(state == Game::Paused) {
        ui->actionPlayPause->setEnabled(true);
        ui->actionPlayPause->setText(tr("Play"));
        m_resumeWidget->setVisible(true);

        if(m_game->rounds().size() > 1)
            ui->actionStop_Game->setEnabled(true);
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
    m_dialogController->showDialog(widget);
}

void GameWindow::onNewRoundStarted()
{
    enableActionsBasedOnState();

    ui->graphWidget->updateGraphs();
    m_gameOverViewModel->updateViews();
    m_informationModel->updateViews();

    if(GameSettings::instance().gamePercentageWarning()) {
        int oldPercentage = (m_game->rounds().size()-2) * 100 / m_game->totalRoundCount();
        if(oldPercentage < 50 && m_game->completedPercentage() >= 50) {
            QMessageBox box(this);
            box.setPalette(Tools::darkPalette(&box));
            box.setText(tr("This game is 50% completed!"));
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
    m_dialogController->showDialog(dialog);
}

void GameWindow::on_actionAdd_Hochzeit_triggered()
{
    NewRoundDialog *dialog = new NewRoundDialog(this);
    dialog->setRound(m_game->currentRound());
    dialog->setCurrentPage(Round::Hochzeit);
    m_dialogController->showDialog(dialog);
}

void GameWindow::on_actionAdd_Solo_triggered()
{
    NewRoundDialog *dialog = new NewRoundDialog(this);
    dialog->setRound(m_game->currentRound());
    dialog->setCurrentPage(Round::Solo);
    m_dialogController->showDialog(dialog);
}

void GameWindow::on_actionAdd_Trumpfabgabe_triggered()
{
    NewRoundDialog *dialog = new NewRoundDialog(this);
    dialog->setRound(m_game->currentRound());
    dialog->setCurrentPage(Round::Trumpfabgabe);
    m_dialogController->showDialog(dialog);
}

void GameWindow::on_actionAdd_schmeisserei_triggered()
{
    SchmeissereiDialog *dialog = new SchmeissereiDialog(this);
    dialog->setGame(m_game);
    m_dialogController->showDialog(dialog);
}

void GameWindow::on_actionAdd_drinks_triggered()
{
    DrinksWidget *drinksWidget = new DrinksWidget(ui->centralwidget);
    drinksWidget->setRound(m_game->currentRound());
    m_dialogController->showDialog(drinksWidget);
}


void GameWindow::on_actionSet_comment_triggered()
{
    CommentWidget* commentWidget = new CommentWidget(this);
    commentWidget->setGame(m_game);
    m_dialogController->showDialog(commentWidget);
}


void GameWindow::on_actionStats_triggered()
{
    GameStatsWidget* widget = new GameStatsWidget(this);
    widget->setGames(QList<QSharedPointer<Game> >() << m_game);
    widget->setMinimumWidth(800);
    m_dialogController->showDialog(widget);
}

void GameWindow::on_actionSettings_triggered()
{
    SettingsWidget* setWidget = new SettingsWidget(this);
    m_dialogController->showDialog(setWidget);
}

void GameWindow::onTableViewOverviewDoubleClicked(const QModelIndex& index)
{
    int roundIndex = index.column();
    int rowIndex = index.row();

    if(roundIndex < 0 || roundIndex >= m_game->rounds().size())
        return;

    QSharedPointer<Round> round = m_game->rounds().at(roundIndex);
    NewRoundDialog* dlg = new NewRoundDialog(this);
    if(roundIndex == m_game->rounds().size()-1) {
         dlg->setRound(round, NewRoundDialog::NewRound);

         if(rowIndex == m_game->players().size()) {
             dlg->setCurrentPage(Round::NormalRound);
             ui->actionAdd_round->setChecked(true);
         }
         else if(rowIndex == m_game->players().size() + 1) {
             dlg->setCurrentPage(Round::Hochzeit);
             ui->actionAdd_Hochzeit->setChecked(true);
         }
         else if(rowIndex == m_game->players().size() + 2) {
             dlg->setCurrentPage(Round::Solo);
             ui->actionAdd_Solo->setChecked(true);
         }
         else if(rowIndex == m_game->players().size() + 3) {
             dlg->setCurrentPage(Round::Trumpfabgabe);
             ui->actionAdd_Trumpfabgabe->setChecked(true);
         }
         else if(rowIndex == m_game->players().size() + 6) {
             on_actionAdd_drinks_triggered();
             return;
         }
         else if(rowIndex == m_game->players().size() + 5) {
             on_actionAdd_schmeisserei_triggered();
             return;
         }

         m_dialogController->showDialog(dlg);
    }
    else {
        dlg->setRound(round, NewRoundDialog::EditRound);
        m_dialogController->showDialog(dlg);

        switch(round->type()) {
        case Round::NormalRound:
            ui->actionAdd_round->setChecked(true);
            break;
        case Round::Hochzeit:
            ui->actionAdd_Hochzeit->setChecked(true);
            break;
        case Round::Solo:
            ui->actionAdd_Solo->setChecked(true);
            break;
        case Round::Trumpfabgabe:
            ui->actionAdd_Trumpfabgabe->setChecked(true);
            break;
        }
    }
}


/**********************************************************
 * PRE-Game
 */

bool sortPlaces(const QSharedPointer<Place> &p1, const QSharedPointer<Place> &p2)
{
    QList<QSharedPointer<Game>> g1 = p1->games();
    if(g1.isEmpty())
        return false;

    QList<QSharedPointer<Game>> g2 = p2->games();
    if(g2.isEmpty())
        return true;

    return g1.last()->creationTime() > g2.last()->creationTime();
}

void GameWindow::on_pushButtonAddPlayers_clicked()
{
    QList<QSharedPointer<Player>> players = Qp::readAll<Player>();
    qSort(players.begin(), players.end(), sortPlayersByLastGame);

    PlayersListWidget *list = new PlayersListWidget(this);
    list->setPalette(palette());
    list->setAttribute(Qt::WA_MacShowFocusRect, false);
    list->add(players);
    list->setFixedWidth(300);

    foreach(QSharedPointer<Player> p, ui->listWidgetPlayers->items())
        list->remove(p);

    connect(list, &PlayersListWidget::playerActivated,
            this, &GameWindow::addPlayerToGame);

    m_dialogController->setDialogOffsetTop(ui->widgetCreateGame->layout()->contentsMargins().top() + 1);
    m_dialogController->setDialogOffsetLeft(ui->listWidgetPlayers->width());
    m_dialogController->setDialogHeight(ui->listWidgetPlayers->height() + ui->pushButtonAddPlayers->height() - 1);
    m_dialogController->showDialog(list);
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
    game->setAdditionalMissingPlayer(Game::MissingInTurn);

    foreach(QSharedPointer<Player> p, ui->listWidgetPlayers->items()) {
        game->addPlayer(p);
    }

    game->setType(Game::Doppelkopf);
    game->startNextRound();
    setGame(game);
    game->setState(Game::Running);
}

void GameWindow::on_actionCheck_for_updates_triggered()
{
    Updater::instanceForPlatform()->checkForUpdates();
}

void GameWindow::on_actionZoom_in_triggered()
{
    m_gameOverViewModel->setFontSize(m_gameOverViewModel->fontSize() + 1);
    m_informationModel->setFontSize(m_informationModel->fontSize() + 1);
    updateSizes();
}

void GameWindow::on_actionZoom_out_triggered()
{
    m_gameOverViewModel->setFontSize(m_gameOverViewModel->fontSize() - 1);
    m_informationModel->setFontSize(m_informationModel->fontSize() - 1);
    updateSizes();
}

void GameWindow::on_actionNew_Drink_triggered()
{
    DrinkInformationDialog dlg;
    dlg.exec();
}
