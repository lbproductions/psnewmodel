#include "drinkslistwidget.h"

DrinksListWidget::DrinksListWidget(QWidget *parent) :
    ObjectListWidget<Drink>(parent)
{
    connect(this, &ObjectListWidget<Drink>::objectActivated,
            this, &DrinksListWidget::onObjectActivated);
}


void DrinksListWidget::onObjectActivated(QSharedPointer<QObject> object)
{
    emit drinkActivated(qSharedPointerCast<Drink>(object));
}


QListWidgetItem *DrinksListWidget::createItem(QSharedPointer<Drink> object)
{
    QListWidgetItem* newItem = new QListWidgetItem;
    newItem->setText(object->name());
    newItem->setIcon(QIcon(object->picture()));
    return newItem;
}
