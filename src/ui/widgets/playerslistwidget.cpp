#include "playerslistwidget.h"

#include "playerslistdelegate.h"

#include <data/player.h>

PlayersListWidget::PlayersListWidget(QWidget *parent) :
    ObjectListWidget<Player>(parent)
{
    setItemDelegate(new PlayersListDelegate(this));

    connect(this, &ObjectListWidget<Player>::objectActivated,
            this, &PlayersListWidget::onObjectActivated);
}


void PlayersListWidget::onObjectActivated(QSharedPointer<QObject> object)
{
    emit playerActivated(qSharedPointerCast<Player>(object));
}


QListWidgetItem *PlayersListWidget::createItem(QSharedPointer<Player> object)
{
    QListWidgetItem* newItem = new QListWidgetItem;
    newItem->setText(object->name());
    QPixmap pm(16,16);
    pm.fill(object->color());
    newItem->setIcon(QIcon(pm));
    return newItem;
}
