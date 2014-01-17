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

    m_drinkLabel = new QLabel(getDrinkString(), this);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("Drinks:"), this), 0,0);
    layout->addWidget(m_drinkLabel, 0, 1);

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
