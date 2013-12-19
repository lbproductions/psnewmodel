#include "leagueclassementmodel.h"

#include <data/league.h>
#include <data/playerstatistics.h>
#include <data/player.h>
#include <data/matchday.h>

#include <misc/tools.h>

#include <QDebug>

LeagueClassementModel::LeagueClassementModel(QObject *parent):
    QAbstractTableModel(parent)
{
}

QSharedPointer<League> LeagueClassementModel::league() const
{
    return m_league;
}

void LeagueClassementModel::setLeague(const QSharedPointer<League> &league)
{
    beginResetModel();
    m_league = league;
    endResetModel();
}

Qt::ItemFlags LeagueClassementModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled;
}

int LeagueClassementModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    if(!m_league)
        return 0;

    return 5;
}

int LeagueClassementModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    if(!m_league)
        return 0;

    return m_league->players().size();
}

QVariant LeagueClassementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
        return QVariant();

    if(role == Qt::DisplayRole) {
        return m_league->currentMatchday()->playersSortedByAverage().at(section)->name();
    }
    else if(role == Qt::DecorationRole) {
        return m_league->currentMatchday()->playersSortedByAverage().at(section)->color();
    }


    return QVariant();
}

void LeagueClassementModel::setChanged()
{
    emit layoutChanged();
}

QVariant LeagueClassementModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QSharedPointer<Player> player = m_league->currentMatchday()->playersSortedByAverage().at(index.row());
    if(!player)
        return QVariant();

    if(index.column() == 0) {
        int placementMatchdayBefore = -1;
        if(League::currentMatchDayNumber > 1)
            placementMatchdayBefore = m_league->matchdays().at(League::currentMatchDayNumber-2)->placementOfPlayer(player);
        int placementMatchdayCurrent = m_league->currentMatchday()->placementOfPlayer(player);

        if(placementMatchdayBefore < placementMatchdayCurrent) {
            return QPixmap(":/leaguewindow/placedown.png");
        }
        else if(placementMatchdayBefore == -1 || placementMatchdayBefore == placementMatchdayCurrent) {
            return QPixmap(":/leaguewindow/placestay.png");
        }
        else if(placementMatchdayCurrent < placementMatchdayBefore) {
            return QPixmap(":/leaguewindow/placeup.png");
        }
    }
    else if(index.column() == 1) {
        if(role == Qt::DisplayRole) {
            return m_league->playerStats(player)->games().size();
        }
    }
    else if(index.column() == 2) {
        if(role == Qt::DisplayRole) {
            return m_league->playerStats(player)->wins().size();
        }
    }
    else if(index.column() == 3) {
        if(role == Qt::DisplayRole) {
            return m_league->playerStats(player)->losses().size();
        }
    }
    else if(index.column() == 4) {
        if(role == Qt::DisplayRole) {
            return QString("%1").arg(m_league->currentMatchday()->average(player), 4, 'f', 2);
        }
    }


    return QVariant();
}
