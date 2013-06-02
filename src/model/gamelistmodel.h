#ifndef GAMELISTMODEL_H
#define GAMELISTMODEL_H

#include "../lib/QPersistence/src/abstractobjectlistmodel.h"

#include <data/game.h>

class GameListModel : public QpAbstractObjectListModel<Game>
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

private:
    QVariant percentageString(double percentage) const;
    QVariant playersString(const QList<QSharedPointer<Player> > &players) const;
};

#endif // GAMELISTMODEL_H
