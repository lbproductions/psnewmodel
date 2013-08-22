#include "adddrinkswidget.h"

#include <data/player.h>
#include <data/drink.h>
#include <data/livedrink.h>
#include <data/round.h>
#include <data/game.h>
#include <ui/widgets/drinkslistwidget.h>
#include <misc/tools.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

AddDrinksWidget::AddDrinksWidget(QWidget *parent) :
    QWidget(parent)
{
    m_layoutPlayerWidgets = new QHBoxLayout(this);
    m_layoutPlayerWidgets->setSpacing(0);
    m_layoutPlayerWidgets->setContentsMargins(0,0,0,0);
    setLayout(m_layoutPlayerWidgets);
}

void AddDrinksWidget::setPlayers(QList<QSharedPointer<Player> > players)
{

    foreach(QSharedPointer<Player> player, players) {
        QWidget *widget = createPlayerWidget(player);

        m_layoutPlayerWidgets->addWidget(widget);
    }

}

QWidget *AddDrinksWidget::createPlayerWidget(QSharedPointer<Player> player)
{
    // Player name
    QLabel *label = new QLabel(player->name());
    QLabel *labelPixmap = new QLabel();
    labelPixmap->setFixedSize(QSize(16,16));
    labelPixmap->setPixmap(player->colorPixmap());
    QLayout *labelLayout = new QHBoxLayout();
    labelLayout->addWidget(labelPixmap);
    labelLayout->addWidget(label);
    labelLayout->setContentsMargins(12,12,12,12);
    labelLayout->setSpacing(12);

    // Drinks list
    DrinksListWidget *drinksListWidget = new DrinksListWidget();
    drinksListWidget->add(player->drinks());
    drinksListWidget->setAttribute(Qt::WA_MacShowFocusRect, false);
    drinksListWidget->setFrameStyle(QFrame::NoFrame);

    connect(drinksListWidget, &DrinksListWidget::drinkActivated, [=]() {
        QSharedPointer<LiveDrink> ld = Qp::create<LiveDrink>();
        ld->setPlayer(player);
        ld->setDrink(drinksListWidget->current());
        m_game->addDrink(ld);
        Qp::update(ld);
        Qp::update(m_game->currentRound());
    });

    //QVBoxLayout *liveDrinksLayout = new QVBoxLayout();
    if(m_game) {
        QList<QSharedPointer<LiveDrink> > liveDrinks = m_game->liveDrinks(player);
        if(!liveDrinks.isEmpty())
            drinksListWidget->setCurrent(liveDrinks.last()->drink());

        /*
        // LiveDrinks list
        QMap<QSharedPointer<Drink>, int> drinkCounts = m_game->drinkCounts(player);
        for(auto it = drinkCounts.constBegin(); it != drinkCounts.constEnd(); ++it) {
            QLabel *drinkLabel = new QLabel(QString("%1x %2")
                                            .arg(it.value())
                                            .arg(it.key()->name()));
            liveDrinksLayout->addWidget(drinkLabel);
        }
        */
    }



    // Complete Widget
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);
    widget->setLayout(layout);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    layout->addWidget(drinksListWidget);
    layout->addLayout(labelLayout);
    //layout->addLayout(liveDrinksLayout);

    return widget;
}

QSharedPointer<Game> AddDrinksWidget::game() const
{
    return m_game;
}

void AddDrinksWidget::setGame(const QSharedPointer<Game> &game)
{
    m_game = game;
}
