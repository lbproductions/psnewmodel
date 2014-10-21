#include "gamestatswidget.h"
#include "ui_gamestatswidget.h"

#include <misc/tools.h>
#include <data/game.h>
#include <data/league.h>

#include <ui/stats/recontrastatswidget.h>
#include <ui/stats/pointsstatswidget.h>
#include <ui/stats/drinkstatswidget.h>
#include <ui/stats/gamestogetherwidget.h>
#include <ui/stats/solooverviewwidget.h>
#include <ui/widgets/gameswidget.h>
#include <ui/stats/gamecomparestatswidget.h>
#include <ui/stats/timestatswidget.h>
#include <ui/stats/servestatswidget.h>
#include <ui/stats/seriesstatswidget.h>
#include <ui/stats/placestatswidget.h>
#include <QSettings>
#include <misc/tools.h>

GameStatsWidget::GameStatsWidget(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::GameStatsWidget),
    m_widgetCounter(0)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("GameStats"));
    this->setAutoFillBackground(true);
    this->setPalette(Tools::darkPalette(this));

    m_reContraWidget = new ReContraStatsWidget(this);
    addWidget(tr("Re/Contra"), m_reContraWidget);

    m_pointsStatsWidget = new PointsStatsWidget(this);
    addWidget(tr("Points"), m_pointsStatsWidget);

    m_gamesTogetherWidget = new GamesTogetherWidget(this);
    addWidget(tr("Games together"), m_gamesTogetherWidget);

    m_soloWidget = new SoloOverviewWidget(this);
    addWidget(tr("Soli"), m_soloWidget);

    m_drinksWidget = new DrinkStatsWidget(this);
    addWidget(tr("Drinks"), m_drinksWidget);

    //m_gameCompareStatsWidget = new GameCompareStatsWidget(this);
    //addWidget(tr("GameCompare"), m_gameCompareStatsWidget);

    m_timeStatsWidget = new TimeStatsWidget(this);
    addWidget(tr("TimeStats"), m_timeStatsWidget);

    m_serveStatsWidget = new ServeStatsWidget(this);
    addWidget(tr("ServeStats"), m_serveStatsWidget);

    m_seriesStatsWidget = new SeriesStatsWidget(this);
    addWidget(tr("SeriesStats"), m_seriesStatsWidget);

    ui->treeWidget->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeWidget->setPalette(Tools::darkPalette(this));

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onItemClicked(QTreeWidgetItem*)));

    setAttribute(Qt::WA_DeleteOnClose, true);
}

GameStatsWidget::~GameStatsWidget()
{
    delete ui;
}

void GameStatsWidget::setGames(QList<QSharedPointer<Game> > games)
{
    m_games = games;

    m_reContraWidget->setGames(games);

    m_pointsStatsWidget->StatsTreeWidget::setGames(games);

    m_gamesTogetherWidget->StatsTreeWidget::setGames(games);

    m_soloWidget->setGames(games);

    m_drinksWidget->setGames(games);

    m_timeStatsWidget->setGames(games);

    m_serveStatsWidget->setGames(games);

    m_seriesStatsWidget->setGames(games);
}

void GameStatsWidget::setLeague(QSharedPointer<League> league)
{
    if(!league)
        return;

    m_games = league->calculatedGames();

    m_reContraWidget->setLeague(league);

    m_pointsStatsWidget->setLeague(league);

    m_gamesTogetherWidget->setLeague(league);

    m_soloWidget->setLeague(league);

    m_drinksWidget->setLeague(league);

    //m_gameCompareStatsWidget->setLeague(league);

    m_timeStatsWidget->setLeague(league);

    m_serveStatsWidget->setLeague(league);

    m_seriesStatsWidget->setLeague(league);

    m_placeStatsWidget = new PlaceStatsWidget(this);
    addWidget(tr("Places"), m_placeStatsWidget);
    m_placeStatsWidget->setLeague(league);
}

void GameStatsWidget::onItemClicked(QTreeWidgetItem *item)
{
    ui->stackedWidget->setCurrentIndex(m_indexes.value(item));
}

void GameStatsWidget::addWidget(QString name, QWidget *widget)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0, name);
    ui->treeWidget->addTopLevelItem(item);
    ui->stackedWidget->insertWidget(m_widgetCounter, widget);
    m_indexes.insert(item, m_widgetCounter);
    m_widgetCounter++;
}
