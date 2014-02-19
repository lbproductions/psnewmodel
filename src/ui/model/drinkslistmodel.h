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
    explicit DrinksSortFilterModel(DrinksListModel *sourceModel, QObject *parent = 0);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const Q_DECL_OVERRIDE;
};

#endif // DRINKSLISTMODEL_H
