#include "playerstatswidget.h"

#include <QVBoxLayout>

#include <data/player.h>
#include <data/game.h>
#include <data/drink.h>

#include <misc/tools.h>

PlayerStatsWidget::PlayerStatsWidget(QSharedPointer<Player> player, QSharedPointer<Game> game, QWidget *parent) :
    QWidget(parent),
    m_game(game),
    m_player(player)
{
    connect(m_game.data(), SIGNAL(newRoundStarted()), this, SLOT(update()));
    connect(m_game.data(), SIGNAL(liveDrinkAdded()), this, SLOT(update()));

    m_drinkLabel = new QLabel(getDrinkString(), this);
    //m_roundsTogetherLabel = new QLabel(getRoundsTogetherString(), this);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* drinkLabel = new QLabel(tr("Drinks:"), this);
    layout->addWidget(drinkLabel);
    layout->addWidget(m_drinkLabel);

    //layout->addWidget(new QLabel(tr("Rounds/Wins together:"), this));
    //layout->addWidget(m_roundsTogetherLabel);

    this->setLayout(layout);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(51,51,51));
    this->setPalette(pal);
    this->setAutoFillBackground(true);
}

void PlayerStatsWidget::update()
{
    m_drinkLabel->setText(getDrinkString());
    //m_roundsTogetherLabel->setText(getRoundsTogetherString());
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

    return drinks;

}

QString PlayerStatsWidget::getRoundsTogetherString()
{
    QString value = "";
    foreach(QSharedPointer<Player> playerWith, m_game->players()) {
        if(m_player != playerWith) {
            value += m_player->name() + " - " + playerWith->name() + "\t" +
                     QString::number(m_game->roundsTogether(m_player, playerWith)) + "\t" +
                     QString::number(m_game->winsTogether(m_player, playerWith))   + "\n";
        }
    }

    return value;
}
