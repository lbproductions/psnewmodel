#include "gameinformationmodel.h"

#include "gameoverviewmodel.h"

#include <data/game.h>
#include <data/player.h>

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

    return m_game->players().size() + GameOverviewModel::ExtraRowsCount;
}

QVariant GameInformationModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

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
    else if(row < m_game->players().size()) {
        QSharedPointer<Player> player = m_game->players().at(row);
        return m_game->totalPoints(player);
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
    else {
        if(role == Qt::DisplayRole) {
            return m_game->players().at(section)->name();
        }
        else if(role == Qt::DecorationRole) {
            return m_game->players().at(section)->color();
        }
    }

    return QVariant();
}
