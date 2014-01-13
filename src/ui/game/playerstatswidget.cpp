#include "playerstatswidget.h"

#include <QGridLayout>

#include <data/playerstatistics.h>
#include <data/player.h>
#include <data/game.h>
#include <data/drink.h>

#include <misc/tools.h>

PlayerStatsWidget::PlayerStatsWidget(QSharedPointer<Player> player, QSharedPointer<Game> game, QWidget *parent) :
    QWidget(parent),
    m_player(player),
    m_game(game)
{
    connect(m_game.data(), SIGNAL(newRoundStarted()), this, SLOT(update()));
    connect(m_game.data(), SIGNAL(liveDrinkAdded()), this, SLOT(update()));

    m_playerStats = new PlayerStatistics(this);
    m_playerStats->setPlayer(player.data());
    m_playerStats->setGames(QList<QSharedPointer<Game> >() << game);

    m_drinkLabel = new QLabel(getDrinkString(), this);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("Drinks:"), this), 0,0);
    layout->addWidget(m_drinkLabel, 0, 1);

    m_reContraBar = new QProgressBar(this);
    m_reContraBar->setStyleSheet(Tools::progressBarStyleSheet(percentageColor(50)));
    m_reContraBar->setAlignment(Qt::AlignCenter);
    m_reContraBar->setMaximumHeight(20);
    layout->addWidget(m_reContraBar, 1,0, 1,2);

    m_reWinBar = new QProgressBar(this);
    m_reWinBar->setStyleSheet(Tools::progressBarStyleSheet(percentageColor(50)));
    m_reWinBar->setAlignment(Qt::AlignCenter);
    m_reWinBar->setMaximumHeight(20);
    layout->addWidget(m_reWinBar, 2,0, 1,2);

    m_contraWinBar = new QProgressBar(this);
    m_contraWinBar->setStyleSheet(Tools::progressBarStyleSheet(percentageColor(50)));
    m_contraWinBar->setAlignment(Qt::AlignCenter);
    m_contraWinBar->setMaximumHeight(20);
    layout->addWidget(m_contraWinBar, 3,0, 1,2);

    this->setLayout(layout);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(51,51,51));
    this->setPalette(pal);
    this->setAutoFillBackground(true);

    update();
}

void PlayerStatsWidget::update()
{
    m_drinkLabel->setText(getDrinkString());

    int rePercentage = m_playerStats->rePercentage();
    m_reContraBar->setValue(rePercentage);
    m_reContraBar->setFormat("RePercentage: " + QString::number(rePercentage) + "%");
    m_reContraBar->setStyleSheet(Tools::progressBarStyleSheet(percentageColor(rePercentage)));

    int reWinPercentage = m_playerStats->reWinsPercentage();
    m_reWinBar->setValue(reWinPercentage);
    m_reWinBar->setFormat("ReWinPercentage: " + QString::number(reWinPercentage) + "%");
    m_reWinBar->setStyleSheet(Tools::progressBarStyleSheet(percentageColor(reWinPercentage)));

    int contraWinPercentage = m_playerStats->contraWinsPercentage();
    m_contraWinBar->setValue(contraWinPercentage);
    m_contraWinBar->setFormat("ContraWinPercentage: " + QString::number(contraWinPercentage) + "%");
    m_contraWinBar->setStyleSheet(Tools::progressBarStyleSheet(percentageColor(contraWinPercentage)));
}

QColor PlayerStatsWidget::percentageColor(double percentage)
{
    if(percentage < 25) {
        return QColor("red");
    }
    else if(percentage < 50) {
        return QColor(123,0,0);
    }
    else if(percentage < 75) {
        return QColor(0, 123, 0);
    }
    else {
        return QColor(0,200,0);
    }
}

QString PlayerStatsWidget::getDrinkString()
{
    QMap<QSharedPointer<Drink>, int> drinkCounts = m_game->drinkCounts(m_player);

    if(drinkCounts.isEmpty()) {
        return "";
    }

    QString drinks = "";
    for(auto it = drinkCounts.constBegin(); it != drinkCounts.constEnd(); ++it) {
        drinks += QString("%1x %2 \n")
                .arg(it.value())
                .arg(it.key()->name());
    }

    drinks = drinks.remove(drinks.size()-2,2);

    return drinks;

}
