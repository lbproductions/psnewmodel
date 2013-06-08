#include "gamewindow.h"
#include "ui_gamewindow.h"

#include "overviewplayerheaderview.h"
#include "overviewhorizontalheaderview.h"
#include "overviewdelegate.h"
#include "newrounddialog.h"
#include "schmeissereidialog.h"
#include "statisticswidget.h"

#include <ui/widgets/playerslistwidget.h>
#include <ui/widgets/menubar.h>
#include <ui/widgets/popupwidget.h>
#include <data/game.h>
#include <data/place.h>
#include <model/gameoverviewmodel.h>
#include <data/player.h>
#include <misc/tools.h>

#include <QPersistence.h>
#include <QScrollBar>
#include <QWheelEvent>
#include <QWindow>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow),
    m_gameOverViewModel(new GameOverviewModel(this)),
    m_popupWidget(nullptr)
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

    // This has a bug in 5.0
    // TODO: enable in Qt 5.1 final: https://bugreports.qt-project.org/browse/QTBUG-31061
    //    ui->listWidgetPlayers->setDragDropMode(QAbstractItemView::InternalMove);

    ui->comboBoxSite->addPlaces(Qp::readAll<Place>());
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->listWidgetPlayers->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidgetPlayers->setPalette(darkPalette);
    ui->graphAxis->setFixedWidth(ui->listWidgetPlayers->width() + 6);


    ui->tableView->hide();
    ui->splitter->setPalette(darkPalette);
    ui->tableView->setPalette(darkPalette);
    ui->tableView->setModel(m_gameOverViewModel);
    OverviewPlayerHeaderView *verticalHeaderView = new OverviewPlayerHeaderView(Qt::Vertical, this);
    verticalHeaderView->setGameModel(m_gameOverViewModel);
    OverviewHorizontalHeaderView *horizontalHeaderView = new OverviewHorizontalHeaderView(Qt::Horizontal, this);
    OverviewDelegate *delegate = new OverviewDelegate(this);
    delegate->setGameModel(m_gameOverViewModel);
    ui->tableView->setVerticalHeader(verticalHeaderView);
    ui->tableView->setHorizontalHeader(horizontalHeaderView);
    ui->tableView->setItemDelegate(delegate);

    connect(ui->scrollAreaGraph->horizontalScrollBar(), &QScrollBar::valueChanged,
            ui->tableView->horizontalScrollBar(), &QScrollBar::setValue);

    ui->graphWidget->setPalette(darkPalette);
    ui->graphAxis->setPalette(darkPalette);
    ui->graphAxis->setGraph(ui->graphWidget);
    ui->graphWidget->stackUnder(ui->graphAxis);

    ui->scrollAreaGraph->addFixedWidget(ui->graphAxis);

    ui->widgetStatisticsFrame->setWidget(new StatisticsWidget(ui->widgetStatisticsFrame));

    QTimer *lengthTimer = new QTimer(this);
    connect(lengthTimer, &QTimer::timeout,
            this, &GameWindow::updateTimes);
    lengthTimer->start(1000);

    ui->toolButtonState->setDefaultAction(ui->actionPlayPause);
    ui->toolButtonAddRound->setDefaultAction(ui->actionAdd_round);
    ui->toolButtonAddSchmeisserei->setDefaultAction(ui->actionAdd_schmeisserei);
    ui->toolButtonToggleSidebar->setDefaultAction(ui->actionToggleSidebar);
    ui->toolButtonToggleSidebar->setText(QString());

    connect(ui->splitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(setSidebarToggleActionBasedUponSidebarState()));

    enableActionsBasedOnState();
    setSidebarToggleToHide();
    MenuBar::instance()->addAction(tr("&Game"), ui->actionPlayPause, this);
    MenuBar::instance()->addAction(tr("&Game"), ui->actionAdd_round, this);
    MenuBar::instance()->addAction(tr("&Game"), ui->actionAdd_schmeisserei, this);
    MenuBar::instance()->addAction(tr("&View"), ui->actionToggleSidebar, this);
    updateTimes();
}

GameWindow::~GameWindow()
{
    if(m_game) {
        m_game->pause();
        m_game->save();
    }
    delete ui;
}

QSharedPointer<Game> GameWindow::game() const
{
    return m_game;
}

void GameWindow::setGame(const QSharedPointer<Game> &game)
{
    ui->widgetCreateGame->hide();
    ui->tableView->show();
    m_game = game;
    m_gameOverViewModel->setGame(game);
    ui->tableView->setFixedHeight(ui->tableView->horizontalHeader()->height() +
                                  (m_gameOverViewModel->rowCount()) * ui->tableView->rowHeight(0));
    ui->graphWidget->setGame(game);
    ui->graphAxis->setFixedWidth(m_gameOverViewModel->extraColumnCount() * 40 + ui->tableView->verticalHeader()->width());

    enableActionsBasedOnState();
    updateTimes();
}

void GameWindow::wheelEvent(QWheelEvent *e)
{
    if(e->pixelDelta().x() > 0)
        ui->scrollAreaGraph->horizontalScrollBar()->setValue(ui->scrollAreaGraph->horizontalScrollBar()->value() + e->pixelDelta().x());
    else
        ui->scrollAreaGraph->horizontalScrollBar()->setValue(ui->scrollAreaGraph->horizontalScrollBar()->value() - e->pixelDelta().y());
}

void GameWindow::mousePressEvent(QMouseEvent *e)
{
    if(popupWidget()) {
        if(!popupWidget()->geometry().adjusted(30,30,-30,-30).contains(e->pos())) {
            popupWidget()->close();
            popupWidget()->deleteLater();
            setPopupWidget(nullptr);
        }
    }

    QMainWindow::mousePressEvent(e);
}

void GameWindow::resizeEvent(QResizeEvent *)
{
    if(popupWidget()) {
        popupWidget()->close();
        popupWidget()->deleteLater();
        setPopupWidget(nullptr);
    }
}

void GameWindow::on_actionPlayPause_triggered()
{
    m_game->togglePlayPause();
    m_game->save();
    enableActionsBasedOnState();
}

void GameWindow::enableActionsBasedOnState()
{
    ui->actionPlayPause->setEnabled(false);
    ui->actionAdd_round->setEnabled(false);
    ui->actionAdd_schmeisserei->setEnabled(false);

    if(!m_game)
        return;

    Game::State state = m_game->state();
    if(state == Game::Running) {
        ui->actionAdd_round->setEnabled(true);
        ui->actionAdd_schmeisserei->setEnabled(true);
        ui->actionPlayPause->setEnabled(true);
        ui->actionPlayPause->setText(tr("Pause"));
        ui->toolButtonState->setIcon(QIcon(":/statusbar/pause.png"));
    }
    else if(state == Game::Paused) {
        ui->actionPlayPause->setEnabled(true);
        ui->actionPlayPause->setText(tr("Play"));
        ui->toolButtonState->setIcon(QIcon(":/statusbar/play.png"));
    }
    else {
        ui->actionPlayPause->setText(tr("Pause"));
    }

}

void GameWindow::on_actionAdd_round_triggered()
{
    if(popupWidget()) {
        popupWidget()->close();
    }

    PopupWidget *popup = new PopupWidget(this);

    NewRoundDialog *dialog = new NewRoundDialog(popup);
    dialog->setRound(m_game->currentRound());
    dialog->setWindowFlags(Qt::Widget);

    popup->setWidget(dialog);
    popup->setMinimumWidth(600);
    popup->setMinimumHeight(500);
    popup->anchorTo(ui->toolButtonAddRound);
    popup->show();
    setPopupWidget(popup);

    connect(dialog, &QDialog::accepted,
            ui->graphWidget, &GraphWidget::updateGraphs);
}

void GameWindow::on_actionAdd_schmeisserei_triggered()
{
    if(popupWidget()) {
        popupWidget()->close();
    }
    PopupWidget *popup = new PopupWidget(this);

    SchmeissereiDialog *dialog = new SchmeissereiDialog(popup);
    dialog->setRound(m_game->currentRound());
    dialog->setWindowFlags(Qt::Widget);

    popup->setWidget(dialog);
    popup->setMinimumWidth(300);
    popup->setMinimumHeight(200);
    popup->anchorTo(ui->toolButtonAddSchmeisserei);
    popup->show();
    setPopupWidget(popup);
}

void GameWindow::updateTimes()
{
    ui->labelClock->setText(QTime::currentTime().toString());

    if(m_game)
        ui->labelLength->setText(m_game->length().toString());
}

void GameWindow::on_actionToggleSidebar_triggered()
{
    if(ui->widgetStatisticsFrame->isVisible()) {
        QList<int> sizes = ui->splitter->sizes();
        if(sizes.last() == 0) {
            sizes[sizes.size() - 1] = 100;
            ui->splitter->setSizes(sizes);
            setSidebarToggleToHide();
        }
        else {
            ui->widgetStatisticsFrame->hide();
            setSidebarToggleToShow();
        }
    }
    else {
        ui->widgetStatisticsFrame->show();
        setSidebarToggleToHide();
    }
}

void GameWindow::setSidebarToggleActionBasedUponSidebarState()
{
    if(ui->widgetStatisticsFrame->isVisible()
            && ui->splitter->sizes().last() > 0) {
        setSidebarToggleToHide();
    }
    else {
        setSidebarToggleToShow();
    }
}

void GameWindow::setSidebarToggleToHide()
{
    ui->actionToggleSidebar->setText(tr("Hide sidebar"));
    ui->toolButtonToggleSidebar->setText(QString());
    ui->toolButtonToggleSidebar->setStyleSheet("QToolButton#toolButtonToggleSidebar {"
                                               "border-image: url(:/statusbar/sidebar-right.png);"
                                               "}"
                                               "QToolButton#toolButtonToggleSidebar:pressed {"
                                               "border-image: url(:/statusbar/sidebar-right-pressed.png);"
                                               "}");
}

void GameWindow::setSidebarToggleToShow()
{
    ui->actionToggleSidebar->setText(tr("Show sidebar"));
    ui->toolButtonToggleSidebar->setText(QString());
    ui->toolButtonToggleSidebar->setStyleSheet("QToolButton#toolButtonToggleSidebar {"
                                               "border-image: url(:/statusbar/sidebar-left.png);"
                                               "}"
                                               "QToolButton#toolButtonToggleSidebar:pressed {"
                                               "border-image: url(:/statusbar/sidebar-left-pressed.png);"
                                               "}");
}

void GameWindow::on_pushButtonAddPlayers_clicked()
{
    if(popupWidget()) {
        popupWidget()->close();
    }

    PopupWidget *popup = new PopupWidget(this);

    PlayersListWidget *list = new PlayersListWidget(popup);
    list->setPalette(palette());
    list->setAttribute(Qt::WA_MacShowFocusRect, false);
    list->setPlayers(Qp::readAll<Player>());

    foreach(QSharedPointer<Player> p, ui->listWidgetPlayers->players())
        list->removePlayer(p);

    connect(list, &PlayersListWidget::playerActivated,
            this, &GameWindow::addPlayerToGame);

    popup->setWidget(list);
    popup->setMinimumWidth(400);
    popup->setMinimumHeight(500);
    popup->setArrowPosition(PopupWidget::Left);
    popup->anchorTo(ui->pushButtonAddPlayers);
    popup->show();
    setPopupWidget(popup);
}

void GameWindow::addPlayerToGame(QSharedPointer<Player> player)
{
    PlayersListWidget *list = static_cast<PlayersListWidget *>(sender());
    list->removePlayer(player);
    ui->listWidgetPlayers->addPlayer(player);
    if(ui->listWidgetPlayers->count() >= 4)
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

PopupWidget *GameWindow::popupWidget() const
{
    return m_popupWidget;
}

void GameWindow::setPopupWidget(PopupWidget *extraWidget)
{
    m_popupWidget = extraWidget;
}

void GameWindow::on_buttonBox_accepted()
{
    QSharedPointer<Game> game = Qp::create<Game>();
    game->setMitPflichtSolo(ui->checkBoxMitPflichtSoli->isChecked());
    game->setSite(ui->comboBoxSite->currentPlace());

    foreach(QSharedPointer<Player> p, ui->listWidgetPlayers->players()) {
        game->addPlayer(p);
    }

    game->setType(Game::Doppelkopf);
    game->startNextRound();
    game->setState(Game::Running);
    setGame(game);
}
