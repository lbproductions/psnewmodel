#include "leaguelistmodel.h"

LeagueListModel::LeagueListModel(QObject *parent) :
    QpObjectListModel<League>(parent)
{
}

int LeagueListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int index = metaObject()->indexOfEnumerator("Columns");
    return metaObject()->enumerator(index).keyCount();
}

QVariant LeagueListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    switch(section) {
    case NameColumn:
        return tr("Name");
    case StartDateColumn:
        return tr("StartDate");
    case EndDateColumn:
        return tr("EndDate");
    case PlayerRatioColumn:
        return tr("PlayerRatio");
    default:
        break;
    }

    return QVariant();
}

QVariant LeagueListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QSharedPointer<League> league = objectByIndex(index);

    if(role == Qt::DisplayRole) {
        switch(index.column()) {
        case NameColumn:
            return league->name();
        case StartDateColumn:
            return league->startDate().toString(Qt::SystemLocaleDate);
        case EndDateColumn:
            return league->endDate().toString(Qt::SystemLocaleDate);
        case PlayerRatioColumn:
            return league->playerRatio();
        default:
            break;
        }
    }

    return QVariant();
}

LeagueSortFilterModel::LeagueSortFilterModel(LeagueListModel *sourceModel, QObject *parent) :
    QpSortFilterProxyObjectModel<League>(sourceModel, parent)
{
}

bool LeagueSortFilterModel::filterAcceptsObject(QSharedPointer<League>) const
{
    return true;
}

bool LeagueSortFilterModel::lessThan(QSharedPointer<League> left, QSharedPointer<League> right) const
{
    if(sortRole() == Name)
        return left->name() > right->name();

    return QpSortFilterProxyObjectModel<League>::lessThan(left, right);
}
