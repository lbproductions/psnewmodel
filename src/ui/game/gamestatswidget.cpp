#include "gamestatswidget.h"
#include "ui_gamestatswidget.h"

#include <ui/widgets/gamestogetherwidget.h>
#include <misc/tools.h>
#include <data/game.h>

#include "recontrastatswidget.h"
#include "pointsstatswidget.h"

#include <QSettings>
#include <misc/tools.h>

GameStatsWidget::GameStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameStatsWidget),
    m_widgetCounter(0)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("GameStats"));
    this->setPalette(Tools::darkPalette(this));

    m_reContraWidget = new ReContraStatsWidget(this);
    addWidget(tr("Re/Contra"), m_reContraWidget);

    m_pointsStatsWidget = new PointsStatsWidget(this);
    addWidget(tr("Points"), m_pointsStatsWidget);

    m_gamesTogetherWidget = new GamesTogetherWidget(this);
    addWidget(tr("Games together"), m_gamesTogetherWidget);

    ui->treeWidget->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onItemClicked(QTreeWidgetItem*)));

    setAttribute(Qt::WA_DeleteOnClose, true);
}

GameStatsWidget::~GameStatsWidget()
{
    delete ui;
}

void GameStatsWidget::setGame(QSharedPointer<Game> game)
{
    m_game = game;

    m_reContraWidget->setGame(game);

    m_pointsStatsWidget->setGame(game);

    m_gamesTogetherWidget->setGames(QList<QSharedPointer<Game> >() << game);
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
