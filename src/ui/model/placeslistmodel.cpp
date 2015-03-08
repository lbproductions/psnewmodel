#include "placeslistmodel.h"

#include <data/player.h>
#include <misc/tools.h>

PlacesListModel::PlacesListModel(QObject *parent) :
    QpObjectListModel(parent)
{
}

QVariant PlacesListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    switch(section) {
    case PostalCodeColumn:
        return tr("Postal Code");
    case CityColumn:
        return tr("City");
    case CityEmblemColumn:
        return tr("");
    case StreetColumn:
        return tr("Street");
    case HouseNumberColumn:
        return tr("Number");
    case PlayersColumn:
        return tr("Players");
    case GameCountColumn:
        return tr("#Games");
    case CommentColumn:
        return tr("Comment");
    default:
        break;
    }

    return QVariant();
}

QVariant PlacesListModel::data(const QModelIndex &index, int role) const
{

    if(!index.isValid())
        return QVariant();

    QSharedPointer<Place> place = objectByIndex(index);

    QStringList players;

    if(role == Qt::DisplayRole) {
        switch(index.column()) {
        case PostalCodeColumn:
            return place->postalCode();
        case CityColumn:
            return place->city();
        case CityEmblemColumn:
            return place->cityEmblem().scaledToHeight(40, Qt::SmoothTransformation);
        case StreetColumn:
            return place->street();
        case HouseNumberColumn:
            return place->houseNumber();
        case PlayersColumn:
            foreach(QSharedPointer<Player> player, place->players()) {
                players << player->name();
            }
            return players.join(", ");
        case GameCountColumn:
            return place->games().size();
        case CommentColumn:
            return place->comment();
        default:
            break;
        }
    }

    if(role == Qt::DecorationRole) {
        QPixmap pm;
        QList<QSharedPointer<Player>> list;

        switch(index.column()) {
        case CityEmblemColumn:
            return place->cityEmblem();
        case PlayersColumn:
            list = place->players();
            QList<QColor> colors;
            foreach(QSharedPointer<Player> p , list) {
                colors << p->color();
            }
            pm = Tools::colorListPixmap(9,9,colors);
            return pm;
        }
    }

    return QVariant();
}

PlacesSortFilterModel::PlacesSortFilterModel(QObject *parent) :
    QpSortFilterProxyObjectModel<Place>(new PlacesListModel(parent), parent)
{
}

bool PlacesSortFilterModel::filterAcceptsObject(QSharedPointer<Place> place) const
{
    Q_UNUSED(place)

    return true;
}

bool PlacesSortFilterModel::lessThan(QSharedPointer<Place> left, QSharedPointer<Place> right) const
{
    if(sortRole() == Name)
        return left->displayString() < right->displayString();

    if(sortRole() == Games)
        return left->games().size() < right->games().size();

    return QpSortFilterProxyObjectModel<Place>::lessThan(left, right);
}
