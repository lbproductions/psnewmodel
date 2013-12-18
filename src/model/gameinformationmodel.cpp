#include "gameinformationmodel.h"

#include "gameoverviewmodel.h"

#include <data/game.h>
#include <data/player.h>

#include <misc/settings.h>

GameInformationModel::GameInformationModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

QSharedPointer<Game> GameInformationModel::game() const
{
    return m_game;
}

void GameInformationModel::setGame(const QSharedPointer<Game> &game)
{
    beginResetModel();
    m_game = game;
    connect(m_game.data(), SIGNAL(newRoundStarted()), this, SIGNAL(layoutChanged()));
    connect(m_game.data(), SIGNAL(schmeissereiAdded()), this, SIGNAL(layoutChanged()));
    connect(&GameSettings::instance(), SIGNAL(playerSortChanged(PlayerSort)), this, SIGNAL(layoutChanged()));
    connect(&GameSettings::instance(), SIGNAL(pointsDisplayChanged(PointsDisplay)), this, SIGNAL(layoutChanged()));
    connect(&GameSettings::instance(), SIGNAL(showExtraRowsChanged(bool)), this, SIGNAL(layoutChanged()));
    endResetModel();
}

Qt::ItemFlags GameInformationModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled;
}

int GameInformationModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    if(!m_game)
        return 0;

    return 1;
}

int GameInformationModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    if(!m_game)
        return 0;

    if(GameSettings::instance().showExtraRows()) {
        return m_game->players().size() + GameOverviewModel::ExtraRowsCount;
    }
    else{
        return m_game->players().size();
    }
}

QVariant GameInformationModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == GameOverviewModel::TotalPointsRole) {
        return true;
    }
    else if(role == Qt::DisplayRole) {

        int row = index.row();
        int extraRow = row - m_game->players().size();

        if(extraRow == GameOverviewModel::HochzeitenRow) {
            return m_game->hochzeitCount();
        }
        else if(extraRow == GameOverviewModel::TrumpfabgabenRow) {
            return m_game->trumpfabgabeCount();
        }
        else if(extraRow == GameOverviewModel::SoliRow) {
            return m_game->soloCount();
        }
        else if(extraRow == GameOverviewModel::SchweinereienRow) {
            return m_game->schweinereiCount();
        }
        else if(extraRow == GameOverviewModel::SchmeissereiRow) {
            return m_game->schmeissereiCount();
        }
        else if(row < m_game->players().size()) {
            QSharedPointer<Player> player;
            if(GameSettings::instance().playerSort() == GameSettings::SortByPosition) {
                player = m_game->players().at(row);
            }
            else if(GameSettings::instance().playerSort() == GameSettings::SortByPlacement) {
                player = m_game->playersSortedByPlacement().at(row);
            }
            if(GameSettings::instance().pointsDisplay() == GameSettings::DifferenceToLeader) {
                return -m_game->pointsToLeader(player);
            }
            else if(GameSettings::instance().pointsDisplay() == GameSettings::TotalPoints) {
                return m_game->totalPoints(player);
            }

        }
    }

    return QVariant();
}

QVariant GameInformationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
        return QVariant();

    int extraRow = section - m_game->players().size();

    if(extraRow == GameOverviewModel::HochzeitenRow) {
        return tr("Hochzeiten");
    }
    else if(extraRow == GameOverviewModel::TrumpfabgabenRow) {
        return tr("Trumpfabgaben");
    }
    else if(extraRow == GameOverviewModel::SoliRow) {
        return tr("Soli");
    }
    else if(extraRow == GameOverviewModel::SchweinereienRow) {
        return tr("Schweinereien");
    }
    else if(extraRow == GameOverviewModel::SchmeissereiRow) {
        return tr("Schmeissereien");
    }
    else {
        if(role == Qt::DisplayRole) {
            if(GameSettings::instance().playerSort() == GameSettings::SortByPosition) {
                return m_game->players().at(section)->name();
            }
            else if(GameSettings::instance().playerSort() == GameSettings::SortByPlacement) {
                return m_game->playersSortedByPlacement().at(section)->name();
            }
        }
        else if(role == Qt::DecorationRole) {
            if(GameSettings::instance().playerSort() == GameSettings::SortByPosition) {
                return m_game->players().at(section)->color();
            }
            else if(GameSettings::instance().playerSort() == GameSettings::SortByPlacement) {
                return m_game->playersSortedByPlacement().at(section)->color();
            }
        }
    }

    return QVariant();
}
