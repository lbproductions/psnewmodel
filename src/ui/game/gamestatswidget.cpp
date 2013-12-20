#include "gamestatswidget.h"
#include "ui_gamestatswidget.h"

#include <ui/widgets/gamestogetherwidget.h>
#include <misc/tools.h>
#include <data/game.h>

#include <QSettings>

GameStatsWidget::GameStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameStatsWidget),
    m_predictedTimeLeftShown(true),
    m_roundAverageWeighted(false)
{
    ui->setupUi(this);

    connect(ui->labelTimeToPlay, &ClickableLabel::pressed,
            this, &GameStatsWidget::togglePredictedTime);
    connect(ui->labelPredictedTimeLabel, &ClickableLabel::pressed,
            this, &GameStatsWidget::togglePredictedTime);

    connect(ui->labelAverageRound, &ClickableLabel::pressed,
            this, &GameStatsWidget::toggleWeightedAverageRoundTime);
    connect(ui->labelAverageRoundLabel, &ClickableLabel::pressed,
            this, &GameStatsWidget::toggleWeightedAverageRoundTime);

    // read negated values and toggle once to reflect correct state
    QSettings settings;
    m_predictedTimeLeftShown = !settings.value("gamewindow/statswidget/predictedTimeLeftShown", true).toBool();
    m_roundAverageWeighted = !settings.value("gamewindow/statswidget/roundAverageWeighted", false).toBool();
    toggleWeightedAverageRoundTime();
    togglePredictedTime();
}

GameStatsWidget::~GameStatsWidget()
{
    delete ui;

    QSettings settings;
    settings.setValue("gamewindow/statswidget/predictedTimeLeftShown", m_predictedTimeLeftShown);
    settings.setValue("gamewindow/statswidget/roundAverageWeighted", m_roundAverageWeighted);
}

void GameStatsWidget::setGame(QSharedPointer<Game> game)
{
    m_game = game;

    connect(m_game.data(), &Game::lengthChanged,
            this, &GameStatsWidget::update);

    update();
}

void GameStatsWidget::update()
{
    if(!m_game)
        return;

    double weight = 0.5;
    if(m_roundAverageWeighted) {
        weight = 0.7;
    }

    QTime roundLength = m_game->averageRoundLength(weight);

    QTime predictedTime;
    if(m_predictedTimeLeftShown) {
        predictedTime = m_game->predictedTimeToPlay(weight);
    }
    else {
        predictedTime = m_game->predictedEndTime(weight);
    }

    ui->labelRoundsToPlay->setText(QString::number(m_game->roundsToPlay()));
    ui->labelTimeToPlay->setText(predictedTime.toString("hh:mm:ss"));
    ui->labelReContra->setText(QString::number(m_game->reWinsCount()) + " : " + QString::number(m_game->contraWinCount()));
    ui->labelAverageRound->setText(roundLength.toString("hh:mm:ss"));

    if (ui->verticalLayoutGamesTogether) {
        QLayoutItem* item;
        while ((item = ui->verticalLayoutGamesTogether->takeAt(0))) {
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

void GameStatsWidget::togglePredictedTime()
{
    if(m_predictedTimeLeftShown) {
        m_predictedTimeLeftShown = false;
        ui->labelPredictedTimeLabel->setText(tr("Predicted end:"));
        ui->labelPredictedTimeLabel->setToolTip(tr("Click to show predicted time left"));
    }
    else {
        m_predictedTimeLeftShown = true;
        ui->labelPredictedTimeLabel->setText(tr("Predicted time to play:"));
        ui->labelPredictedTimeLabel->setToolTip(tr("Click to show predicted end time"));
    }

    ui->labelTimeToPlay->setToolTip(ui->labelPredictedTimeLabel->toolTip());

    update();
}

void GameStatsWidget::toggleWeightedAverageRoundTime()
{
    if(m_roundAverageWeighted) {
        m_roundAverageWeighted = false;
        ui->labelAverageRoundLabel->setText(tr("Average round length:"));
        ui->labelAverageRoundLabel->setToolTip(tr("Click to show weighted average"));
    }
    else {
        m_roundAverageWeighted = true;
        ui->labelAverageRoundLabel->setText(tr("Weighted round length:"));
        ui->labelAverageRoundLabel->setToolTip(tr("Click to show unweighted"));
    }

    ui->labelAverageRound->setToolTip(ui->labelAverageRoundLabel->toolTip());

    update();
}
