#ifndef DRINKSLISTWIDGET_H
#define DRINKSLISTWIDGET_H

#include "objectlistwidget.h"

#include <data/drink.h>

class DrinksListWidget : public ObjectListWidget<Drink>
{
    Q_OBJECT
public:
    explicit DrinksListWidget(QWidget *parent = 0);

signals:
    void drinkActivated(QSharedPointer<Drink> drink);

protected:
    QListWidgetItem *createItem(QSharedPointer<Drink> object);

private slots:
    void onObjectActivated(QSharedPointer<QObject> object);

    
};

#endif // DRINKSLISTWIDGET_H
