#ifndef PLAYERSLISTMODEL_H
#define PLAYERSLISTMODEL_H

#include "../lib/QPersistence/src/abstractobjectlistmodel.h"

#include <data/player.h>

class PlayersListModel : public QpAbstractObjectListModel<Player>
{
    Q_OBJECT
    Q_ENUMS(Columns)
public:
    enum Columns {
        AvatarColumn,
        ColorColumn,
        NameColumn,

        GameCountColumn,
        LossesColumn,
        WinsColumn,

        LastGameColumn,
        LastWinColumn,

        GamePointsColumn,

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

#endif // PLAYERSLISTMODEL_H

