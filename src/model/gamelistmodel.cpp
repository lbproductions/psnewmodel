#include "gamelistmodel.h"

#include <data/place.h>
#include <data/player.h>
#include <misc/tools.h>

GameListModel::GameListModel(QObject *parent) :
    QpAbstractObjectListModel<Game>(parent)
{
}

int GameListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return metaObject()->enumerator(0).keyCount();
}

QVariant GameListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    switch(section) {
    case StateColumn:
        return tr("");
    case NameColumn:
        return tr("Name");
    case DateColumn:
        return tr("Date");
    case LengthColumn:
        return tr("Length");
    case PercentageComplete:
        return tr("%Complete");
    case PlayerCountColumn:
        return tr("#Players");
    case PlayersColumn:
        return tr("Players");
    case SiteColumn:
        return tr("Site");
    case DrinkCount:
        return tr("#Drinks");
    case TotalPointsColumn:
        return tr("Points");
    case RoundCountColumn:
        return tr("#Rounds");
    case ReWinRoundsCountColumn:
        return tr("#Re Wins");
    case ContraWinRoundsCountColumn:
        return tr("#Contra Wins");
    case HochzeitCountColumn:
        return tr("#Hochzeiten");
    case SchmeissereienCountColumn:
        return tr("#Schmeissereien");
    case SchweinereienCountColumn:
        return tr("#Schweinereien");
    case SoloCountColumn:
        return tr("#Soli");
    case TrumpfabgabenColumn:
        return tr("#Trumpfabgaben");
    default:
        break;
    }

    return QVariant();
}

QVariant GameListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QSharedPointer<Game> game = objectByIndex(index);

    if(role == Qt::DisplayRole) {
        switch(index.column()) {
        case NameColumn:
            return game->name();
        case DateColumn:
            return game->creationTime().date();
        case LengthColumn:
            return game->length();
        case PercentageComplete:
            return Tools::percentageString(game->completedPercentage());
        case PlayerCountColumn:
            return game->players().size();
        case PlayersColumn:
            return playersString(game->players());
        case SiteColumn:
            return game->site()->displayString();
        case DrinkCount:
            return game->drinks().size();
        case TotalPointsColumn:
            return game->totalPoints();
        case RoundCountColumn:
            return game->rounds().size();
        case ReWinRoundsCountColumn:
            return 0;
        case ContraWinRoundsCountColumn:
            return 0;
        case HochzeitCountColumn:
            return game->hochzeitCount();
        case SchmeissereienCountColumn:
            return game->schmeissereiCount();
        case SchweinereienCountColumn:
            return game->schweinereiCount();
        case SoloCountColumn:
            return game->soloCount();
        case TrumpfabgabenColumn:
            return game->trumpfabgabeCount();
        case StateColumn:
        default:
            break;
        }
    }

    if(role == Qt::DecorationRole) {

        switch(index.column()) {
        case StateColumn:
            return game->statePixmap();
        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool playersAlphabetically(const QSharedPointer<Player> &p1, const QSharedPointer<Player> &p2)
{
    return p1->name() < p2->name();
}

QVariant GameListModel::playersString(const QList<QSharedPointer<Player> > &ps) const
{
    if(ps.isEmpty())
        return QVariant();

    QStringList list;
    foreach(QSharedPointer<Player> p, ps) {
        list.append(p->name());
    }

    return list.join(", ");
}
