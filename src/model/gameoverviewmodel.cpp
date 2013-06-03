#include "gameoverviewmodel.h"

#include <data/game.h>
#include <data/round.h>
#include <data/player.h>

#include <QDebug>

GameOverviewModel::GameOverviewModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_extraColumnCount = 1;
    m_extraRowsCount = 1;
}

QSharedPointer<Game> GameOverviewModel::game() const
{
    return m_game;
}

void GameOverviewModel::setGame(const QSharedPointer<Game> &game)
{
    beginResetModel();
    m_game = game;
    endResetModel();
}

Qt::ItemFlags GameOverviewModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled;
}

int GameOverviewModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    if(!m_game)
        return 0;

    return m_game->totalRoundCount() + 1;
}

int GameOverviewModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    if(!m_game)
        return 0;

    return m_game->players().size() + 1;
}

QVariant GameOverviewModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.column() >= m_game->rounds().size())
        return QVariant();

    int row = index.row();
    int column = index.column();
    int roundIndex = column - m_extraColumnCount;
    int playerIndex = row - m_extraRowsCount;

    if(row < m_extraRowsCount) {
        if(row == HochzeitenRow) {
            if(column == TotalColumn) {
                return m_game->hochzeitCount();
            }

            QSharedPointer<Round> round = m_game->rounds().at(roundIndex);

            QSharedPointer<Player> hochzeitPlayer = round->hochzeitPlayer();
            if(hochzeitPlayer) {
                if(role == Qt::DecorationRole)
                    return hochzeitPlayer->color();

                if(role == HochzeitMitspielerColorRole) {
                    QSharedPointer<Player> mitspieler = round->re1Player();
                    if(mitspieler == hochzeitPlayer)
                        mitspieler = round->re2Player();

                    if(mitspieler)
                        return mitspieler->color();
                }
            }

        }
    }
    else if(column < m_extraColumnCount) {
        if(column == TotalColumn) {
            if(row == HochzeitenRow) {
                return QVariant();
            }

            QSharedPointer<Player> player = m_game->players().at(playerIndex);
            return m_game->totalPoints(player);
        }
    }
    else {
        QSharedPointer<Player> player = m_game->players().at(playerIndex);
        QSharedPointer<Round> round = m_game->rounds().at(roundIndex);

        if(round->playingPlayers().contains(player)) {
            if(role == Qt::DisplayRole)
                return round->points(player);
        }
    }

    return QVariant();
}

QVariant GameOverviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal) {
        if(role == Qt::DisplayRole) {
            if(section == 0) {
                return QVariant();
            }
            else {
                int round = section - 1;
                if(round % m_game->players().size() == 0)
                    return round + 1;
            }
        }
    }
    else {
        int row = section;
        int playerIndex = row - m_extraRowsCount;

        if(row < m_extraRowsCount) {
            if(row == HochzeitenRow) {
                return QVariant("Hochzeiten");
            }
        }
        else {
            if(role == Qt::DisplayRole) {
                return m_game->players().at(playerIndex)->name();
            }
            else if(role == Qt::DecorationRole) {
                return m_game->players().at(playerIndex)->color();
            }
        }
    }

    return QVariant();
}
