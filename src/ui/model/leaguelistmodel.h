#ifndef LEAGUELISTMODEL_H
#define LEAGUELISTMODEL_H

#include "../lib/QPersistence/src/objectlistmodel.h"

#include <data/league.h>

class LeagueListModel : public QpObjectListModel<League>
{
    Q_OBJECT
    Q_ENUMS(Columns)
public:
    enum Columns {
        NameColumn,
        StartDateColumn,
        EndDateColumn,
        PlayerRatioColumn
    };

    explicit LeagueListModel(QObject *parent = 0);

    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};

class LeagueSortFilterModel : public QpSortFilterProxyObjectModel<League>
{
    Q_OBJECT
public:
    enum SortRole {
        Name = Qt::UserRole + 1
    };

    explicit LeagueSortFilterModel(LeagueListModel *sourceModel, QObject *parent = 0);

protected:
    bool filterAcceptsObject(QSharedPointer<League>) const Q_DECL_OVERRIDE;
    bool lessThan(QSharedPointer<League> left, QSharedPointer<League> right) const Q_DECL_OVERRIDE;
};

#endif // LEAGUELISTMODEL_H
