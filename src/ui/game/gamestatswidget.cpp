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

#include <QToolButton>

GameStatsWidget::GameStatsWidget(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::GameStatsWidget),
    m_widgetCounter(0)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("GameStats"));
    this->setAutoFillBackground(true);
    this->setPalette(Tools::darkPalette(this));

    ui->sidebar->setLayout(new QVBoxLayout(this));
    ui->sidebar->layout()->setContentsMargins(0,0,0,0);
    static_cast<QVBoxLayout*>(ui->sidebar->layout())->addStretch();

    m_reContraWidget = new ReContraStatsWidget(this);
    addWidget(m_reContraWidget, tr("Re/Contra"), QIcon(":/sidebar/games_c.png"));

    m_pointsStatsWidget = new PointsStatsWidget(this);
    addWidget(m_pointsStatsWidget, tr("Points"), QIcon(":/sidebar/players_c.png"));

    m_gamesTogetherWidget = new GamesTogetherWidget(this);
    addWidget(m_gamesTogetherWidget, tr("Rounds\ntogether"), QIcon(":/sidebar/players_c.png"));

    m_soloWidget = new SoloOverviewWidget(this);
    addWidget(m_soloWidget, tr("Soli"), QIcon(":/gamewindow/solo.png"));

    m_drinksWidget = new DrinkStatsWidget(this);
    addWidget(m_drinksWidget, tr("Drinks"), QIcon(":/gamewindow/drink.png"));

    //m_gameCompareStatsWidget = new GameCompareStatsWidget(this);
    //addWidget(tr("GameCompare"), m_gameCompareStatsWidget);

    m_timeStatsWidget = new TimeStatsWidget(this);
    addWidget(m_timeStatsWidget, tr("TimeStats"), QIcon(":/sidebar/players_c.png"));

    m_serveStatsWidget = new ServeStatsWidget(this);
    addWidget(m_serveStatsWidget, tr("ServeStats"), QIcon(":/sidebar/players_c.png"));

    m_seriesStatsWidget = new SeriesStatsWidget(this);
    addWidget(m_seriesStatsWidget, tr("SeriesStats"), QIcon(":/sidebar/players_c.png"));

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

    m_gamesTogetherWidget->StatsTreeWidget::setGames(games);

    m_soloWidget->setGames(games);

    m_drinksWidget->setGames(games);

    m_timeStatsWidget->setGames(games);

    m_serveStatsWidget->setGames(games);

    m_seriesStatsWidget->setGames(games);
}

void GameStatsWidget::setLeague(QSharedPointer<League> _league)
{
    QSharedPointer<League> league = _league;
    if(league == 0 || league.isNull()) {
        return;
    }

    m_games.clear();
    m_games.append(league->calculatedGames());

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
    addWidget(m_placeStatsWidget, tr("Places"), QIcon(":/sidebar/places_c.png"));
    m_placeStatsWidget->setLeague(league);
}

void GameStatsWidget::addWidget(QWidget *widget, QString name, QIcon icon)
{
    QToolButton* button = new QToolButton(widget);
    button->setFixedWidth(70);
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button->setText(name);
    button->setIcon(icon);
    button->setCheckable(true);
    if(ui->stackedWidget->count() == 0) {
        button->setChecked(true);
    }

    static_cast<QVBoxLayout*>(ui->sidebar->layout())->insertWidget(ui->stackedWidget->count(), button);

    ui->stackedWidget->addWidget(widget);

    connect(button, &QToolButton::clicked, [=]() {
        ui->stackedWidget->setCurrentWidget(widget);

        resetAllButtons();
        button->setChecked(true);
    });
}

void GameStatsWidget::resetAllButtons()
{
    for(int i = 0; i<ui->stackedWidget->count(); i++) {
        static_cast<QToolButton*>(static_cast<QVBoxLayout*>(ui->sidebar->layout())->itemAt(i)->widget())->setChecked(false);
    }
}


