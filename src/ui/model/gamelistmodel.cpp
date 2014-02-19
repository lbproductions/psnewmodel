#include "gamelistmodel.h"

#include <data/place.h>
#include <data/player.h>
#include <misc/tools.h>

GameListModel::GameListModel(QObject *parent) :
    QpObjectListModel<Game>(parent)
{
}

int GameListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int index = metaObject()->indexOfEnumerator("Columns");
    return metaObject()->enumerator(index).keyCount();
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
        return tr("Rounds");
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
            return Tools::percentageString(game->completedPercentage(), 0);
        case PlayersColumn:
            return Tools::playersString(game->players());
        case SiteColumn:
            return game->site()->displayString();
        case DrinkCount:
            return game->liveDrinks().size();
        case TotalPointsColumn:
            return game->totalPoints();
        case RoundCountColumn:
            return tr("%1 / %2")
                    .arg(game->finishedRoundCount(), 2)
                    .arg(game->totalRoundCount());
        case ReWinRoundsCountColumn:
            return game->reWinsCount();
        case ContraWinRoundsCountColumn:
            return game->contraWinCount();
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
            if(game->state() == Game::Finished)
                return tr("\u2713");
        default:
            break;
        }
    }

    if(role == Qt::DecorationRole) {
        QPixmap pm;
        QList<QSharedPointer<Player>> list;

        switch(index.column()) {
        case HostColumn:
            list = game->site()->players();
            if(!list.isEmpty()) {
                pm = Tools::colorEllipse(8,8, list.first()->color());
            }
            return pm;
        case PlayerCountColumn:
            list = game->players();
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

bool playersAlphabetically(const QSharedPointer<Player> &p1, const QSharedPointer<Player> &p2)
{
    return p1->name() < p2->name();
}


GameSortFilterModel::GameSortFilterModel(GameListModel *sourceModel, QObject *parent) :
    QpSortFilterProxyObjectModel<Game>(sourceModel, parent)
{
}

bool GameSortFilterModel::filterAcceptsObject(QSharedPointer<Game> game) const
{
    if(filterRole() == UnfinishedStateFilter)
        return game->state() != Game::Finished && game->state() != Game::UnkownState;

    return true;
}

bool GameSortFilterModel::lessThan(QSharedPointer<Game> left, QSharedPointer<Game> right) const
{
    if(sortRole() == Date)
        return left->creationTime() < right->creationTime();

    return QpSortFilterProxyObjectModel<Game>::lessThan(left, right);
}
