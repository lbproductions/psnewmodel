#include "leaguegameplacementmodel.h"

#include <data/league.h>
#include <data/player.h>
#include <data/game.h>
#include <data/matchday.h>

LeagueGamePlacementModel::LeagueGamePlacementModel(QObject *parent):
    QAbstractTableModel(parent)
{
}

QSharedPointer<League> LeagueGamePlacementModel::league() const
{
    return m_league;
}

void LeagueGamePlacementModel::setLeague(const QSharedPointer<League> &league)
{
    beginResetModel();
    m_league = league;
    endResetModel();
}

Qt::ItemFlags LeagueGamePlacementModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled;
}

int LeagueGamePlacementModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    if(!m_league)
        return 0;

    return League::currentMatchDayNumber + 1; // +1 for graph column with placements
}

int LeagueGamePlacementModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    if(!m_league)
        return 0;

    return m_league->players().size();
}

QVariant LeagueGamePlacementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(section >= m_league->calculatedGames().size())
        return QVariant();

    if(orientation == Qt::Horizontal) {
        if(role == Qt::DisplayRole) {
            return m_league->calculatedGames().at(section)->creationTime().date().toString("dd.MM.yy");
        }
    }

    return QVariant();
}

void LeagueGamePlacementModel::setChanged()
{
    emit layoutChanged();
}

QVariant LeagueGamePlacementModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.column() >= m_league->calculatedGames().size())
        return QVariant();

    QSharedPointer<Player> player = m_league->currentMatchday()->playersSortedByAverage().at(index.row());
    QSharedPointer<Game> game = m_league->matchdays().at(index.column())->game();

    if(role == Qt::DisplayRole) {
        if(game->players().contains(player)) {
            return game->placement(player);
        }
        else{
            return "-";
        }
    }
    if(role == Qt::BackgroundRole) {
        if(game->placement(player) == 1) {
            return QColor(218, 165, 032);
        }
        if(game->placement(player) == game->players().size()) {
            return QColor(178,34,34);
        }
    }

    return QVariant();
}
