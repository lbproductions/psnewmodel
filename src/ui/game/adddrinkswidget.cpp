#include "adddrinkswidget.h"

#include <data/player.h>
#include <data/drink.h>
#include <data/livedrink.h>
#include <data/round.h>
#include <data/game.h>
#include <ui/widgets/drinkslistwidget.h>
#include <ui/drinkinformationdialog.h>
#include <misc/tools.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

AddDrinksWidget::AddDrinksWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    m_layoutPlayerWidgets = new QHBoxLayout(this);
    m_layoutPlayerWidgets->setSpacing(0);
    m_layoutPlayerWidgets->setContentsMargins(0,0,0,0);
    layout->addLayout(m_layoutPlayerWidgets);

    //Add drink button
    QHBoxLayout* newDrinkLayout = new QHBoxLayout(this);
    QPushButton* addButton = new QPushButton(tr("New Drink"), this);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", addButton);

    connect(addButton, &QPushButton::clicked, [=]() {
        DrinkInformationDialog* dlg = new DrinkInformationDialog(this);
        dlg->setPalette(Tools::darkPalette(dlg));

        connect(dlg, SIGNAL(drinkAdded(QSharedPointer<Drink>)), this, SLOT(addDrinkToWidgets(QSharedPointer<Drink>)));

        dlg->exec();
    });

    newDrinkLayout->addWidget(addButton);
    newDrinkLayout->setContentsMargins(0,0,0,0);
    layout->addLayout(newDrinkLayout);

    setLayout(layout);
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
    drinksListWidget->add(Qp::readAll<Drink>());
    drinksListWidget->setAttribute(Qt::WA_MacShowFocusRect, false);
    drinksListWidget->setFrameStyle(QFrame::NoFrame);
    m_drinkListWidgets.append(drinksListWidget);

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

void AddDrinksWidget::addDrinkToWidgets(QSharedPointer<Drink> drink)
{
    foreach(DrinksListWidget* widget, m_drinkListWidgets) {
        widget->add(QList<QSharedPointer<Drink> >() << drink);
    }
}

QSharedPointer<Game> AddDrinksWidget::game() const
{
    return m_game;
}

void AddDrinksWidget::setGame(const QSharedPointer<Game> &game)
{
    m_game = game;
}
