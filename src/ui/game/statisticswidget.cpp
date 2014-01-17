#include "statisticswidget.h"
#include "ui_statisticswidget.h"

#include "playerstatswidget.h"
#include "gamestatswidget.h"
#include "settingswidget.h"
#include <ui/widgets/popupwidget.h>

#include <data/game.h>
#include <data/player.h>

StatisticsWidget::StatisticsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget)
{
    ui->setupUi(this);

    this->layout()->setContentsMargins(0,0,0,0);
    this->layout()->setSpacing(0);
    this->layout()->setMargin(0);

    ui->scrollArea->setContentsMargins(0,0,0,0);
}

StatisticsWidget::~StatisticsWidget()
{
    delete ui;
}

void StatisticsWidget::setGame(QSharedPointer<Game> game)
{
    m_game = game;

    QVBoxLayout* layout = new QVBoxLayout(this);

    //GAMESTATS
//    QPushButton* gameButton = new QPushButton(QPixmap(":/sidebar/games.png"),m_game->name()==""?"Game":m_game->name(), this);
//    gameButton->setStyleSheet("color: white; border: 1px solid rgb(108,108,108); height: 20px; font-size: 14px; text-align:left;");
//    connect(gameButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
//    GameStatsWidget* gameWidget = new GameStatsWidget(this);
//    gameWidget->setGame(m_game);
//    m_statsWidgets.insert(gameButton, gameWidget);
//    layout->addWidget(gameButton);
//    layout->addWidget(gameWidget);

    /*
    //SETTINGS
    QPushButton* statsButton = new QPushButton(tr("Settings"),this);
    statsButton->setStyleSheet("color: white; border: 1px solid rgb(108,108,108); height: 20px; font-size: 14px; text-align:left;");
    connect(statsButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
    SettingsWidget* settingsWidget = new SettingsWidget(this);
    m_statsWidgets.insert(statsButton, settingsWidget);
    layout->addWidget(statsButton);
    layout->addWidget(settingsWidget);
    */

    // PLAYERSTATS
    foreach(QSharedPointer<Player> player, game->players()) {
        PlayerStatsWidget* statsWidget = new PlayerStatsWidget(player, game, this);

        QPushButton* button = new QPushButton(player->colorPixmap(),player->name(), this);
        button->setStyleSheet("color: white; border: 1px solid rgb(108,108,108); height: 20px; font-size: 14px; text-align:left;");
        connect(button, SIGNAL(pressed()), this, SLOT(onButtonPressed()));

        m_statsWidgets.insert(button, statsWidget);

        // TODO: Find a way to display triangle correctly
        /*
        QHBoxLayout* splitLayout = new QHBoxLayout(this);
        splitLayout->addStretch();
        QLabel* splitLabel = new QLabel(this);
        splitLabel->setPixmap(QPixmap(":/general/popup-arrow-top.png").scaledToHeight(20));
        splitLabel->setFixedHeight(20);
        splitLayout->addWidget(splitLabel);
        splitLayout->addStretch();
        m_splitLabels.insert(button, splitLabel);
        layout->setSpacing(0);
        */

        layout->addWidget(button);
        //layout->addLayout(splitLayout);
        layout->addWidget(statsWidget);
    }
    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);

    layout->addStretch();

    QWidget* widget = new QWidget(this);
    widget->setContentsMargins(0,0,0,0);
    widget->setLayout(layout);

    ui->scrollArea->setWidget(widget);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}



void StatisticsWidget::onButtonPressed()
{
    QPushButton* button = static_cast<QPushButton*>(sender());
    m_statsWidgets.value(button)->setVisible(!m_statsWidgets.value(button)->isVisible());
    //m_splitLabels.value(button)->setVisible(!m_splitLabels.value(button)->isVisible());

}
