#include "playerslistmodel.h"

#include <data/game.h>
#include <misc/tools.h>

PlayersListModel::PlayersListModel(QObject *parent) :
    QpObjectListModel<Player>(parent)
{
}

int PlayersListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int index = metaObject()->indexOfEnumerator("Columns");
    return metaObject()->enumerator(index).keyCount();
}

QVariant PlayersListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    switch(section) {
        case AvatarColumn:
            return tr("");
        case ColorColumn:
            return tr("");
        case NameColumn:
            return tr("Name");
        case GenderColumn:
            return tr("Gender");
        case HeightColumn:
            return tr("Height");
        case WeightColumn:
            return tr("Weight");
        case GameCountColumn:
            return tr("#Games");
        case GamePointsColumn:
            return tr("Game Points");
        case LastGameColumn:
            return tr("Last Game");
        case LastWinColumn:
            return tr("Last Win");
        case LossesCountColumn:
            return tr("#Losses");
        case WinsCountColumn:
            return tr("#Wins");
        case ContraCountColumn:
            return tr("#Contra");
        case ContraPercentageColumn:
            return tr("%");
        case ContraWinsColumn:
            return tr("#Wins");
        case ContraWinsPercentageColumn:
            return tr("%");
        case ReCountColumn:
            return tr("#Re");
        case RePercentageColumn:
            return tr("%");
        case ReWinsColumn:
            return tr("#Wins");
        case ReWinsPercentageColumn:
            return tr("%");
        case HochzeitenCountColumn:
            return tr("#Hochzeiten");
        case SchmeissereienCountColumn:
            return tr("#Schmeissereien");
        case SchweinereienCountColumn:
            return tr("#Schweinereien");
        case SoloCountColumn:
            return tr("#Soli");
        case TrumpfabgabenCountColumn:
            return tr("#Trumpfabgaben");
        case AverageColumn:
            return tr("Average");
        case PointsColumn:
            return tr("Points");
        default:
            break;
    }

    return QVariant();
}

QVariant PlayersListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QSharedPointer<Player> player = objectByIndex(index);

    QSharedPointer<Game> game;

    if(role == Qt::DisplayRole) {
        switch(index.column()) {
            case NameColumn:
                return player->name();
            case GenderColumn:
                return player->genderString();
            case HeightColumn:
                return Tools::ifPositive(player->height());
            case WeightColumn:
                return Tools::ifPositive(player->weight());
            case GameCountColumn:
                return player->games().size() + player->offlineGameInformation().size();
            case GamePointsColumn:
                return player->gamePoints();
            case LastGameColumn:
                game = player->lastGame();
                if(game)
                    return game->creationTime().date();
                return QDate();
            case LastWinColumn:
                game = player->lastWin();
                if(game)
                    return game->creationTime().date();
                return QDate();
            case LossesCountColumn:
                return player->losses();
            case WinsCountColumn:
                return player->wins();
            case ContraCountColumn:
                return player->contraRounds().size();
            case ContraPercentageColumn:
                return Tools::percentageString(player->contraPercentage());
            case ContraWinsColumn:
                return player->contraWins().size();
            case ContraWinsPercentageColumn:
                return Tools::percentageString(player->contraWinsPercentage());
            case ReCountColumn:
                return player->re1Rounds().size() + player->re2Rounds().size();
            case RePercentageColumn:
                return Tools::percentageString(player->rePercentage());
            case ReWinsColumn:
                return player->reWins().size();
            case ReWinsPercentageColumn:
                return Tools::percentageString(player->reWinsPercentage());
            case HochzeitenCountColumn:
                return player->hochzeitRounds().size();
            case SchmeissereienCountColumn:
                return player->schmeissereis().size();
            case SchweinereienCountColumn:
                return player->schweinereiRounds().size();
            case SoloCountColumn:
                return player->soloRounds().size();
            case TrumpfabgabenCountColumn:
                return player->trumpfabgabeRounds().size();
            case AverageColumn:
                return QString("%1").arg(player->average(), 4, 'f', 2);
            case PointsColumn:
                return player->points();
            case AvatarColumn:
            case ColorColumn:
            default:
                return QVariant();
        }
    }

    if(role == Qt::DecorationRole) {

        switch(index.column()) {
            case AvatarColumn:
                return Tools::scaledPixmap(player->avatar());
            case ColorColumn:
                return player->color();
            default:
                return QVariant();
        }
    }

    return QVariant();
}


PlayersSortFilterModel::PlayersSortFilterModel(QObject *parent) :
    QpSortFilterProxyObjectModel<Player>(new PlayersListModel(parent), parent)
{
}

bool PlayersSortFilterModel::filterAcceptsObject(QSharedPointer<Player> player) const
{
    Q_UNUSED(player)

    return true;
}

bool PlayersSortFilterModel::lessThan(QSharedPointer<Player> left, QSharedPointer<Player> right) const
{
    if(sortRole() == Name)
        return left->name() < right->name();

    if(sortRole() == Games)
        return left->games().size() < right->games().size();

    return QpSortFilterProxyObjectModel<Player>::lessThan(left, right);
}
