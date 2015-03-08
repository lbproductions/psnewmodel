#ifndef PLACESLISTMODEL_H
#define PLACESLISTMODEL_H

#include <QPersistence.h>
#include <data/place.h>

class PlacesListModel : public QpObjectListModel<Place>
{
    Q_OBJECT
    Q_ENUMS(Columns)
public:
    enum Columns {
        CityEmblemColumn,
        PostalCodeColumn,
        CityColumn,
        StreetColumn,
        HouseNumberColumn,
        CommentColumn,
        PlayersColumn,
        GameCountColumn
    };

    explicit PlacesListModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};

class PlacesSortFilterModel : public QpSortFilterProxyObjectModel<Place>
{
    Q_OBJECT
public:

    enum SortRole {
        Name = Qt::UserRole + 1,
        Games
    };

    explicit PlacesSortFilterModel(QObject *parent = 0);

protected:
    bool filterAcceptsObject(QSharedPointer<Place> place) const Q_DECL_OVERRIDE;
    bool lessThan(QSharedPointer<Place> left, QSharedPointer<Place> right) const Q_DECL_OVERRIDE;
};

#endif // PLACESLISTMODEL_H
