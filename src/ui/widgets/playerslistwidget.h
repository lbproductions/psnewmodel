#ifndef PLAYERSLISTWIDGET_H
#define PLAYERSLISTWIDGET_H

#include "objectlistwidget.h"

#include <data/player.h>

class PlayersListWidget : public ObjectListWidget<Player>
{
    Q_OBJECT
public:
    explicit PlayersListWidget(QWidget *parent = 0);

signals:
    void playerActivated(QSharedPointer<Player> player);

protected:
    QListWidgetItem *createItem(QSharedPointer<Player> object);

private slots:
    void onObjectActivated(QSharedPointer<QObject> object);

};

#endif // PLAYERSLISTWIDGET_H
