#include "leaguewindow.h"
#include "ui_leaguewindow.h"

#include <QHBoxLayout>
#include <QTableView>
#include <QScrollBar>
#include <QWheelEvent>

#include <data/league.h>
#include <model/leagueclassementmodel.h>
#include <model/leaguegameplacementmodel.h>

#include "leaguehorizontalheaderview.h"
#include "leagueplacementdelegate.h"
#include "leagueplayerheaderview.h"

LeagueWindow::LeagueWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LeagueWindow),
    m_classementModel(new LeagueClassementModel(this)),
    m_gamePlacementModel(new LeagueGamePlacementModel(this))
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

    ui->tableViewPlayer->setModel(m_classementModel);
    ui->tableViewPlayer->setPalette(darkPalette);

    LeaguePlacementDelegate* placementDelegate = new LeaguePlacementDelegate(this);
    placementDelegate->setLeagueClassementModel(m_classementModel);
    ui->tableViewPlayer->setItemDelegate(placementDelegate);

    LeaguePlayerHeaderView* playerHeaderView = new LeaguePlayerHeaderView(Qt::Vertical, this);
    playerHeaderView->setLeagueClassementModel(m_classementModel);
    ui->tableViewPlayer->setVerticalHeader(playerHeaderView);

    LeagueHorizontalHeaderView* horizontalHeader = new LeagueHorizontalHeaderView(Qt::Horizontal, this);
    ui->tableViewPlayer->setHorizontalHeader(horizontalHeader);

    ui->tableViewPlacement->setModel(m_gamePlacementModel);
    ui->tableViewPlacement->setPalette(darkPalette);
    ui->tableViewPlacement->setItemDelegate(placementDelegate);

    LeagueHorizontalHeaderView* horizontalHeader1 = new LeagueHorizontalHeaderView(Qt::Horizontal, this);
    ui->tableViewPlacement->setHorizontalHeader(horizontalHeader1);

    ui->pushButtonNext->setEnabled(false);

    connect(ui->scrollAreaGraph->horizontalScrollBar(), &QScrollBar::valueChanged,
            ui->tableViewPlacement->horizontalScrollBar(), &QScrollBar::setValue);

    ui->graphWidget->setPalette(darkPalette);
    ui->graphAxis->setPalette(darkPalette);
    ui->graphAxis->setGraph(ui->graphWidget);
    ui->graphWidget->stackUnder(ui->graphAxis);

    ui->scrollAreaGraph->addFixedWidget(ui->graphAxis);
}


LeagueWindow::~LeagueWindow()
{
    delete ui;
}

void LeagueWindow::setLeague(QSharedPointer<League> league)
{
    m_league = league;
    m_classementModel->setLeague(m_league);
    m_gamePlacementModel->setLeague(m_league);

    ui->tableViewPlayer->setFixedWidth(ui->tableViewPlayer->verticalHeader()->width() + 191);
    ui->tableViewPlayer->setFixedHeight(ui->tableViewPlayer->horizontalHeader()->height() +
                                  (m_gamePlacementModel->rowCount()) * ui->tableViewPlayer->rowHeight(0));
    ui->tableViewPlacement->setFixedHeight(ui->tableViewPlacement->horizontalHeader()->height() +
                                  (m_gamePlacementModel->rowCount()) * ui->tableViewPlacement->rowHeight(0));

    ui->labelMatchday->setText(QString::number(League::currentMatchDayNumber));

    ui->graphWidget->setLeague(league);
    ui->graphAxis->setFixedWidth(ui->tableViewPlayer->verticalHeader()->width() + 191);

    ui->soloOverviewWidget->setGames(m_league->calculatedGames());

    ui->reContraPieWidget->setGames(m_league->calculatedGames());

    ui->totalPointsWidget->setLeague(m_league);

    ui->gamesTogetherWidget->setData(m_league->calculatedGames(), m_league->players());

    ui->gamesWidget->setGames(m_league->calculatedGames());

    ui->distanceWidget->setGames(m_league->calculatedGames());
}

void LeagueWindow::on_pushButtonCurrent_clicked()
{
    League::currentMatchDayNumber = m_league->calculatedGames().size();
    ui->labelMatchday->setText(QString::number(League::currentMatchDayNumber));

    ui->pushButtonNext->setEnabled(false);
    ui->pushButtonPrev->setEnabled(true);

    m_classementModel->setChanged();
    m_gamePlacementModel->setChanged();

    ui->graphWidget->updateGraphs();
}

void LeagueWindow::on_pushButtonPrev_clicked()
{
    League::currentMatchDayNumber--;
    ui->labelMatchday->setText(QString::number(League::currentMatchDayNumber));

    ui->pushButtonNext->setEnabled(true);

    if(League::currentMatchDayNumber == 1) {
        ui->pushButtonPrev->setEnabled(false);
    }

    m_classementModel->setChanged();
    m_gamePlacementModel->setChanged();

    ui->graphWidget->updateGraphs();
}

void LeagueWindow::on_pushButtonNext_clicked()
{
    League::currentMatchDayNumber++;
    ui->labelMatchday->setText(QString::number(League::currentMatchDayNumber));

    ui->pushButtonPrev->setEnabled(true);
    if(League::currentMatchDayNumber == m_league->calculatedGames().size()) {
        ui->pushButtonNext->setEnabled(false);
    }

    m_classementModel->setChanged();
    m_gamePlacementModel->setChanged();

    ui->graphWidget->updateGraphs();
}

void LeagueWindow::wheelEvent(QWheelEvent *e)
{
    if(e->pos().y() > ui->scrollAreaGraph->rect().bottomLeft().y())
        return;

    if(e->pixelDelta().x() > 0)
        ui->scrollAreaGraph->horizontalScrollBar()->setValue(ui->scrollAreaGraph->horizontalScrollBar()->value() + e->pixelDelta().x());
    else
        ui->scrollAreaGraph->horizontalScrollBar()->setValue(ui->scrollAreaGraph->horizontalScrollBar()->value() - e->pixelDelta().y());
}

void LeagueWindow::on_comboBoxAverage_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "PlacementGraph") {
        ui->graphWidget->setType(LeagueGraphWidget::PlacementGraph);
    }
    else if(arg1 == "AverageGraph") {
        ui->graphWidget->setType(LeagueGraphWidget::AverageGraph);
    }
    else if(arg1 == "PointsGraph") {
        ui->graphWidget->setType(LeagueGraphWidget::PointsGraph);
    }
}