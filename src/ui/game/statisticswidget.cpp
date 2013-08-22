#include "statisticswidget.h"
#include "ui_statisticswidget.h"

#include "data/game.h"
#include "data/player.h"
#include "data/drink.h"

StatisticsWidget::StatisticsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget)
{
    ui->setupUi(this);
}

StatisticsWidget::~StatisticsWidget()
{
    delete ui;
}

void StatisticsWidget::setGame(QSharedPointer<Game> game)
{
    m_game = game;
    connect(m_game.data(), SIGNAL(newRoundStarted()), this, SLOT(update()));
    connect(m_game.data(), SIGNAL(liveDrinkAdded()), this, SLOT(update()));

    m_drinkLayout = new QVBoxLayout(this);
    foreach(QSharedPointer<Player> player, m_game->players()) {
        QString drinks = getDrinkString(player);

        if(drinks != "") {
            QVBoxLayout* layout = new QVBoxLayout(this);

            layout->addWidget(new QLabel(player->name() + ":", this));

            QLabel* label = new QLabel(drinks, this);
            layout->addWidget(label);
            m_drinkLabels.insert(player, label);

            m_drinkLayout->addLayout(layout);
        }
    }

    static_cast<QVBoxLayout*>(this->layout())->insertLayout(0, m_drinkLayout);

    update();
}

QString StatisticsWidget::getDrinkString(QSharedPointer<Player> player)
{
    QMap<QSharedPointer<Drink>, int> drinkCounts = m_game->drinkCounts(player);

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

void StatisticsWidget::update()
{
    foreach(QSharedPointer<Player> player, m_game->players()) {
        QString drinks = getDrinkString(player);

        if(drinks != "") {
            if(m_drinkLabels.value(player) == 0) {
                QVBoxLayout* layout = new QVBoxLayout(this);

                layout->addWidget(new QLabel(player->name() + ":", this));

                QLabel* label = new QLabel(drinks, this);
                layout->addWidget(label);
                m_drinkLabels.insert(player, label);

                m_drinkLayout->addLayout(layout);
            }
            else{
                m_drinkLabels.value(player)->setText(drinks);
            }
        }
    }
}
