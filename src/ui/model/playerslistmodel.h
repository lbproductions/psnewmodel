#ifndef PLAYERSLISTMODEL_H
#define PLAYERSLISTMODEL_H

#include "../lib/QPersistence/src/objectlistmodel.h"

#include <data/player.h>

class PlayersListModel : public QpObjectListModel<Player>
{
    Q_OBJECT
    Q_ENUMS(Columns)
public:
    enum Columns {
        AvatarColumn,
        ColorColumn,
        NameColumn,

        GameCountColumn,
        WinsCountColumn,
        LossesCountColumn,

        PointsColumn,
        AverageColumn,
        GamePointsColumn,

        LastGameColumn,
        LastWinColumn,

        ContraCountColumn,
        ContraPercentageColumn,
        ContraWinsColumn,
        ContraWinsPercentageColumn,

        ReCountColumn,
        RePercentageColumn,
        ReWinsColumn,
        ReWinsPercentageColumn,

        HochzeitenCountColumn,
        SchmeissereienCountColumn,
        SchweinereienCountColumn,
        SoloCountColumn,
        TrumpfabgabenCountColumn,

        GenderColumn,
        HeightColumn,
        WeightColumn
    };

    explicit PlayersListModel(QObject *parent = 0);
    
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};

class PlayersSortFilterModel : public QpSortFilterProxyObjectModel<Player>
{
    Q_OBJECT
public:

    enum SortRole {
        Name = Qt::UserRole + 1,
        Games
    };

    explicit PlayersSortFilterModel(QObject *parent = 0);

protected:
    bool filterAcceptsObject(QSharedPointer<Player> player) const Q_DECL_OVERRIDE;
    bool lessThan(QSharedPointer<Player> left, QSharedPointer<Player> right) const Q_DECL_OVERRIDE;
};

#endif // PLAYERSLISTMODEL_H

