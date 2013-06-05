#ifndef DRINKSLISTMODEL_H
#define DRINKSLISTMODEL_H

#include "../lib/QPersistence/src/abstractobjectlistmodel.h"

#include <data/drink.h>

class DrinksListModel : public QpAbstractObjectListModel<Drink>
{
    Q_OBJECT
    Q_ENUMS(Columns)
public:
    enum Columns {
        PictureColumn,
        NameColumn,
        VolumeColumn,
        AlcoholByVolume,
        TypeColumn,
        DrinkCountColumn
    };

    explicit DrinksListModel(QObject *parent = 0);

    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

};

#endif // DRINKSLISTMODEL_H
