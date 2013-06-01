#include "playerslistmodel.h"

#include <data/game.h>

PlayersListModel::PlayersListModel(QObject *parent) :
    QpAbstractObjectListModel<Player>(parent)
{
}

int PlayersListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return metaObject()->enumerator(0).keyCount();
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
        return tr("Points");
    case LastGameColumn:
        return tr("Last Game");
    case LastWinColumn:
        return tr("Last Win");
    case LossesColumn:
        return tr("#Losses");
    case WinsColumn:
        return tr("#Wins");
    case ContraCountColumn:
        return tr("#Contra Rounds");
    case ContraPercentageColumn:
        return tr("%Contra Rounds");
    case ContraWinsColumn:
        return tr("#Contra Wins");
    case ContraWinsPercentageColumn:
        return tr("%Contra Wins");
    case ReCountColumn:
        return tr("#Re Rounds");
    case RePercentageColumn:
        return tr("%Re Rounds");
    case ReWinsColumn:
        return tr("#Re Wins");
    case ReWinsPercentageColumn:
        return tr("%Re Wins");
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
            return player->gender();
        case HeightColumn:
            return player->height();
        case WeightColumn:
            return player->weight();
        case GameCountColumn:
            return player->games().size();
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
        case LossesColumn:
            return player->losses();
        case WinsColumn:
            return player->wins();
        case ContraCountColumn:
            return player->contraRounds().size();
        case ContraPercentageColumn:
            return player->contraPercentage();
        case ContraWinsColumn:
            return player->contraWins().size();
        case ContraWinsPercentageColumn:
            return player->contraWinsPercentage();
        case ReCountColumn:
            return player->re1Rounds().size() + player->re2Rounds().size();
        case RePercentageColumn:
            return player->rePercentage();
        case ReWinsColumn:
            return player->reWins().size();
        case ReWinsPercentageColumn:
            return player->reWinsPercentage();
        case HochzeitenCountColumn:
            return player->hochzeitRounds().size();
        case SchmeissereienCountColumn:
            return player->schmeissereien().size();
        case SchweinereienCountColumn:
            return player->schweinereiRounds().size();
        case SoloCountColumn:
            return player->soloRounds().size();
        case TrumpfabgabenCountColumn:
            return player->trumpfabgabeRounds().size();
        case AvatarColumn:
        case ColorColumn:
        default:
            return QVariant();
        }
    }

    if(role == Qt::DecorationRole) {

        switch(index.column()) {
        case AvatarColumn:
            return player->avatar();
        case ColorColumn:
            return player->color();
        default:
            return QVariant();
        }
    }

    return QVariant();
}
