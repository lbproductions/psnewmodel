#include "gameswidget.h"

#include <misc/tools.h>
#include <data/game.h>
#include <data/place.h>
#include <ui/game/gamewindow.h>

#include <QHeaderView>

GamesWidget::GamesWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setPalette(Tools::darkPalette(this));
    this->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());

    this->setColumnCount(6);
    this->setHeaderLabels(QStringList() << tr("State") << tr("Name") <<tr("Players") << tr("Date") << tr("Place") << tr("Completed"));

    setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*,int)));
}

GamesWidget::~GamesWidget()
{
}

void GamesWidget::setGames(QList<QSharedPointer<Game> > games)
{
    m_games = games;

    foreach(QSharedPointer<Game> game, games) {
        this->addTopLevelItem(createItem(game->statePixmap(), game->name(), Tools::playersString(game->playersSortedByPlacement()).toString(),
                                                   game->creationTime().date().toString("dd.MM.yy"), game->site()->displayString(),
                                                   QString::number(game->completedPercentage()) + "%"));
    }

    resizeColumnToContents(0);
    resizeColumnToContents(1);
    resizeColumnToContents(2);
    resizeColumnToContents(3);
    resizeColumnToContents(4);
}

void GamesWidget::onItemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
    int index = this->indexOfTopLevelItem(item);
    QSharedPointer<Game> game = m_games.at(index);
    if(!game)
        return;

    GameWindow *window = new GameWindow(/*this*/);
    window->setGame(game);
    window->show();
}

QTreeWidgetItem *GamesWidget::createItem(QPixmap statePixmap, QString name, QString players, QString date, QString site, QString complete)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this);
    item->setIcon(0, QIcon(statePixmap));
    item->setText(1, name);
    item->setText(2, players);
    item->setText(3, date);
    item->setText(4, site);
    item->setText(5, complete);

    return item;
}
