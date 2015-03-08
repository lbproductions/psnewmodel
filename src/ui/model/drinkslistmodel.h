#ifndef DRINKSLISTMODEL_H
#define DRINKSLISTMODEL_H

#include "../lib/QPersistence/src/objectlistmodel.h"

#include <data/drink.h>

class DrinksListModel : public QpObjectListModel<Drink>
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

class DrinksSortFilterModel : public QpSortFilterProxyObjectModel<Drink>
{
    Q_OBJECT
public:

    enum SortRole {
        Name = Qt::UserRole + 1,
        DrinkCount
    };

    explicit DrinksSortFilterModel(QObject *parent = 0);

protected:
    bool filterAcceptsObject(QSharedPointer<Drink> drink) const Q_DECL_OVERRIDE;
    bool lessThan(QSharedPointer<Drink> left, QSharedPointer<Drink> right) const Q_DECL_OVERRIDE;
};

#endif // DRINKSLISTMODEL_H
