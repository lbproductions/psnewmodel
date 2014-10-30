#include "leaguewindow.h"
#include "ui_leaguewindow.h"

#include <QHBoxLayout>
#include <QTableView>
#include <QScrollBar>
#include <QWheelEvent>

#include <data/league.h>
#include <ui/model/leagueclassementmodel.h>
#include <ui/model/leaguegameplacementmodel.h>

#include <misc/tools.h>
#include <ui/game/gamestatswidget.h>

#include "leaguehorizontalheaderview.h"
#include "leagueplacementdelegate.h"
#include "leagueplayerheaderview.h"

LeagueWindow::LeagueWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LeagueWindow),
    m_classementModel(new LeagueClassementModel(this)),
    m_gamePlacementModel(new LeagueGamePlacementModel(this)),
    m_minimumColumnWidth(20),
    m_maximumColumnWidth(100)
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

    LeagueDelegate* classementDelegate = new LeagueDelegate(this);
    classementDelegate->setModel(LeagueDelegate::ClassementModel);
    classementDelegate->setClassementModel(m_classementModel);
    ui->tableViewPlayer->setItemDelegate(classementDelegate);

    LeaguePlayerHeaderView* playerHeaderView = new LeaguePlayerHeaderView(Qt::Vertical, this);
    playerHeaderView->setLeagueClassementModel(m_classementModel);
    ui->tableViewPlayer->setVerticalHeader(playerHeaderView);

    LeagueHorizontalHeaderView* horizontalHeader = new LeagueHorizontalHeaderView(Qt::Horizontal, this);
    horizontalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
    //horizontalHeader->setFixedHeight(30);
    ui->tableViewPlayer->setHorizontalHeader(horizontalHeader);

    LeagueDelegate* placementDelegate = new LeagueDelegate(this);
    placementDelegate->setModel(LeagueDelegate::PlacementModel);
    placementDelegate->setClassementModel(m_classementModel);
    ui->tableViewPlacement->setModel(m_gamePlacementModel);
    ui->tableViewPlacement->setPalette(darkPalette);
    ui->tableViewPlacement->setItemDelegate(placementDelegate);

    LeagueHorizontalHeaderView* horizontalHeader1 = new LeagueHorizontalHeaderView(Qt::Horizontal, this);
    ui->tableViewPlacement->setHorizontalHeader(horizontalHeader1);

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

    ui->tableViewPlayer->setFixedWidth(ui->tableViewPlayer->verticalHeader()->width() + 171);
    ui->tableViewPlayer->setFixedHeight(ui->tableViewPlayer->horizontalHeader()->height() +
                                  (m_gamePlacementModel->rowCount()) * ui->tableViewPlayer->rowHeight(0));
    ui->tableViewPlacement->setFixedHeight(ui->tableViewPlacement->horizontalHeader()->height() +
                                  (m_gamePlacementModel->rowCount()) * ui->tableViewPlacement->rowHeight(0));

    ui->graphWidget->setLeague(league);
    ui->graphAxis->setFixedWidth(ui->tableViewPlayer->verticalHeader()->width() + 171);

    //ui->gamesWidget->setGames(m_league->calculatedGames());

    //ui->gameStatsWidget->setLeague(m_league);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonStats);
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

void LeagueWindow::resizeEvent(QResizeEvent *)
{
    if(!m_league)
        return;

    int w = width();
    int roundCount = m_league->calculatedGames().size();

    w -= ui->tableViewPlayer->width();
    w /= roundCount + 1;
    w = qMax(m_minimumColumnWidth, w);
    w = qMin(m_maximumColumnWidth, w);

    ui->tableViewPlacement->horizontalHeader()->setDefaultSectionSize(w);
    ui->graphWidget->setColumnWidth(w);

    this->repaint();
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
    else if (arg1 == "TotalPointsGraph") {
        ui->graphWidget->setType(LeagueGraphWidget::TotalPointsGraph);
    }
}

void LeagueWindow::on_doubleSpinBoxFinishedGames_valueChanged(double arg1)
{
    m_league->setFinishedGamesBorder(arg1);
    m_league->recalculate();

    m_classementModel->setChanged();
    m_gamePlacementModel->setChanged();

    ui->graphWidget->updateGraphs();
}

void LeagueWindow::on_pushButtonStats_clicked()
{
    //if(!m_gameStatsWidget) {
        m_gameStatsWidget = new GameStatsWidget(this);
    //}
    m_gameStatsWidget->setMinimumWidth(800);
    m_gameStatsWidget->setLeague(m_league);
    m_gameStatsWidget->show();
}
