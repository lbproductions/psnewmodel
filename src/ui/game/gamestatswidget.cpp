#include "gamestatswidget.h"
#include "ui_gamestatswidget.h"

#include <ui/widgets/gamestogetherwidget.h>
#include <misc/tools.h>
#include <data/game.h>

GameStatsWidget::GameStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameStatsWidget)
{
    ui->setupUi(this);
}

GameStatsWidget::~GameStatsWidget()
{
    delete ui;
}

void GameStatsWidget::setGame(QSharedPointer<Game> game)
{
    m_game = game;

    connect(m_game.data(), SIGNAL(newRoundStarted()), this, SLOT(update()));

    update();
}

void GameStatsWidget::update()
{
    if(!m_game)
        return;

    ui->labelRoundsToPlay->setText(QString::number(m_game->roundsToPlay()));
    ui->labelTimeToPlay->setText(m_game->predictedTimeToPlay().toString("hh:mm:ss"));
    ui->labelReContra->setText(QString::number(m_game->reWinsCount()) + " : " + QString::number(m_game->contraWinCount()));
    ui->labelAverageRound->setText(m_game->averageRoundLength().toString("hh:mm:ss"));

    if ( ui->verticalLayoutGamesTogether != NULL )
    {
        QLayoutItem* item;
        while ( ( item = ui->verticalLayoutGamesTogether->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }

    /*
    //TODO: TreeWidget aus GamesTogetherWidget auslagern und mit update-Methode ausstatten
    GamesTogetherWidget* widget = new GamesTogetherWidget(this);
    widget->setData(QList<QSharedPointer<Game> > () << m_game, m_game->players());
    QTreeWidget* treeWidget = widget->gamesTogetherWidget();
    treeWidget->setParent(this);
    treeWidget->setStyleSheet("QTreeWidget{color:white}");
    treeWidget->setMinimumHeight(treeWidget->topLevelItemCount()*treeWidget->visualItemRect(treeWidget->topLevelItem(0)).height() + 1);
    ui->verticalLayoutGamesTogether->addWidget(treeWidget);
    delete widget;
    */
}
