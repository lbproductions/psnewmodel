#include "drinkslistmodel.h"

#include <misc/tools.h>

DrinksListModel::DrinksListModel(QObject *parent) :
    QpObjectListModel<Drink>(parent)
{
}

int DrinksListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int index = metaObject()->indexOfEnumerator("Columns");
    return metaObject()->enumerator(index).keyCount();
}

QVariant DrinksListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    switch(section) {
    case PictureColumn:
        return tr("");
    case NameColumn:
        return tr("Name");
    case VolumeColumn:
        return tr("Volume");
    case AlcoholByVolume:
        return tr("Vol\u2030");
    case TypeColumn:
        return tr("Type");
    case DrinkCountColumn:
        return tr("Count");
    default:
        break;
    }

    return QVariant();
}

QVariant DrinksListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QSharedPointer<Drink> drink = objectByIndex(index);

    if(role == Qt::DisplayRole) {
        switch(index.column()) {
        case NameColumn:
            return drink->name();
        case VolumeColumn:
            return QString("%1 l").arg(drink->volume());
        case AlcoholByVolume:
            return QString("%1 \u2030").arg(drink->alcoholByVolume());
        case TypeColumn:
            return drink->typeString();
        case DrinkCountColumn:
            return drink->liveDrinks().size();
        default:
            break;
        }
    }

    if(role == Qt::DecorationRole) {
        switch(index.column()) {
        case PictureColumn:
            return Tools::scaledPixmap(drink->picture());
        default:
            return QVariant();
        }
    }

    return QVariant();

}

DrinksSortFilterModel::DrinksSortFilterModel(DrinksListModel *sourceModel, QObject *parent) :
    QpSortFilterProxyObjectModel<Drink>(sourceModel, parent)
{
}

bool DrinksSortFilterModel::filterAcceptsRow(int /*source_row*/, const QModelIndex &/*source_parent*/) const
{
    return true;
}

bool DrinksSortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return QSortFilterProxyModel::lessThan(left, right);
}
