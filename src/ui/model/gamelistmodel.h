#ifndef GAMELISTMODEL_H
#define GAMELISTMODEL_H

#include "../lib/QPersistence/src/objectlistmodel.h"

#include <data/game.h>

class GameListModel : public QpObjectListModel<Game>
{
    Q_OBJECT
    Q_ENUMS(Columns)
public:
    enum Columns {
        StateColumn,
        NameColumn,
        DateColumn,
        LengthColumn,
        PercentageComplete,
        PlayerCountColumn,
        PlayersColumn,
        SiteColumn,
        TotalPointsColumn,
        RoundCountColumn,
        ReWinRoundsCountColumn,
        ContraWinRoundsCountColumn,
        HochzeitCountColumn,
        SchmeissereienCountColumn,
        SchweinereienCountColumn,
        SoloCountColumn,
        TrumpfabgabenColumn,
        DrinkCount
    };

    explicit GameListModel(QObject *parent = 0);

    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};

class GameSortFilterModel : public QpSortFilterProxyObjectModel<Game>
{
    Q_OBJECT
public:
    enum Filter {
        UnfinishedStateFilter = Qt::UserRole + 1
    };

    enum SortRole {
        Date = Qt::UserRole + 1
    };

    explicit GameSortFilterModel(GameListModel *sourceModel, QObject *parent = 0);

protected:
    bool filterAcceptsObject(QSharedPointer<Game> game) const Q_DECL_OVERRIDE;
    bool lessThan(QSharedPointer<Game> left, QSharedPointer<Game> right) const Q_DECL_OVERRIDE;
};

#endif // GAMELISTMODEL_H
