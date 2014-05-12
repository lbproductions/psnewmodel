#include "gamestatswidget.h"
#include "ui_gamestatswidget.h"

#include <misc/tools.h>
#include <data/game.h>
#include <data/league.h>

#include "recontrastatswidget.h"
#include "pointsstatswidget.h"
#include "drinkstatswidget.h"
#include <ui/widgets/gamestogetherwidget.h>
#include <ui/widgets/solooverviewwidget.h>
#include "gamecomparestatswidget.h"
#include "timestatswidget.h"
#include <QSettings>
#include <misc/tools.h>

GameStatsWidget::GameStatsWidget(QWidget *parent) :
    QWidget(parent),
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

    m_gameCompareStatsWidget = new GameCompareStatsWidget(this);
    addWidget(tr("GameCompare"), m_gameCompareStatsWidget);

    m_timeStatsWidget = new TimeStatsWidget(this);
    addWidget(tr("TimeStats"), m_timeStatsWidget);

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

    m_pointsStatsWidget->setGames(games);

    m_gamesTogetherWidget->setGames(games);

    m_soloWidget->setGames(games);

    m_drinksWidget->setGames(games);

    m_timeStatsWidget->setGames(games);
}

void GameStatsWidget::setLeague(QSharedPointer<League> league)
{
    m_games = league->calculatedGames();

    m_reContraWidget->setLeague(league);

    m_pointsStatsWidget->setLeague(league);

    m_gamesTogetherWidget->setLeague(league);

    m_soloWidget->setLeague(league);

    m_drinksWidget->setLeague(league);

    m_gameCompareStatsWidget->setLeague(league);

    m_timeStatsWidget->setLeague(league);
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
