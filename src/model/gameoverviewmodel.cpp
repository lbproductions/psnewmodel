#include "gameoverviewmodel.h"

#include <data/game.h>
#include <data/round.h>
#include <data/player.h>

#include <QDebug>

GameOverviewModel::GameOverviewModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_extraColumnCount = 1;
    m_extraRowsCount = 3;
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

    return m_game->totalRoundCount() + m_extraColumnCount + 1;
}

int GameOverviewModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    if(!m_game)
        return 0;

    return m_game->players().size() + m_extraRowsCount;
}

QVariant GameOverviewModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    int row = index.row();
    int column = index.column();
    int roundIndex = column - m_extraColumnCount;
    int playerIndex = row - m_extraRowsCount;

    if(roundIndex >= m_game->rounds().size())
        return QVariant();

    if(row < m_extraRowsCount) {

        if(row == HochzeitenRow) {
            if(column == TotalColumn) {
                return m_game->hochzeitCount();
            }
            QSharedPointer<Round> round = m_game->rounds().at(roundIndex);

            QSharedPointer<Player> player = round->hochzeitPlayer();
            if(player) {
                if(role == Qt::DecorationRole)
                    return player->color();

                if(role == Qt::BackgroundColorRole)
                    return colorFromPoints(round->points(player));

                if(role == MitspielerColorRole) {
                    QSharedPointer<Player> mitspieler = round->re1Player();
                    if(mitspieler == player)
                        mitspieler = round->re2Player();

                    if(mitspieler)
                        return mitspieler->color();
                }
            }
        }
        else if(row == TrumpfabgabenRow) {
            if(column == TotalColumn) {
                return m_game->trumpfabgabeCount();
            }
            QSharedPointer<Round> round = m_game->rounds().at(roundIndex);

            QSharedPointer<Player> player = round->trumpfabgabePlayer();
            if(player) {
                if(role == Qt::DecorationRole)
                    return player->color();

                if(role == Qt::BackgroundColorRole)
                    return colorFromPoints(round->points(player));

                if(role == MitspielerColorRole) {
                    QSharedPointer<Player> mitspieler = round->re1Player();
                    if(mitspieler == player)
                        mitspieler = round->re2Player();

                    if(mitspieler)
                        return mitspieler->color();
                }
            }
        }
        else if(row == SoliRow) {
            if(column == TotalColumn) {
                return m_game->soloCount();
            }
            QSharedPointer<Round> round = m_game->rounds().at(roundIndex);

            QSharedPointer<Player> player = round->soloPlayer();
            if(player) {
                if(role == Qt::DecorationRole)
                    return player->color();

                if(role == Qt::BackgroundColorRole)
                    return colorFromPoints(round->points(player));

                if(role == SoloTypeRole)
                    return round->soloType();
            }
        }
    }
    else if(column < m_extraColumnCount) {
        if(column == TotalColumn) {
            QSharedPointer<Player> player = m_game->players().at(playerIndex);
            return m_game->totalPoints(player);
        }
    }
    else if(playerIndex < m_game->players().size()
            && roundIndex < m_game->rounds().size()) {
        QSharedPointer<Player> player = m_game->players().at(playerIndex);
        QSharedPointer<Round> round = m_game->rounds().at(roundIndex);

        if(round->playingPlayers().contains(player)) {
            if(role == Qt::DisplayRole)
                return round->points(player);

            if(role == Qt::BackgroundColorRole)
                return colorFromPoints(round->points(player));
        }
    }

    return QVariant();
}

QVariant GameOverviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal) {
        if(role == Qt::DisplayRole) {
            int roundIndex = section - 1;
            if(roundIndex < m_game->rounds().size()) {
                if(roundIndex % m_game->players().size() == 0)
                    return roundIndex + 1;
            }
        }
    }
    else {
        int row = section;
        int playerIndex = row - m_extraRowsCount;

        if(row < m_extraRowsCount) {
            if(row == HochzeitenRow) {
                return tr("Hochzeiten");
            }
            else if(row == TrumpfabgabenRow) {
                return tr("Trumpfabgaben");
            }
            else if(row == SoliRow) {
                return tr("Soli");
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

int GameOverviewModel::extraColumnCount() const
{
    return m_extraColumnCount;
}

void GameOverviewModel::setExtraColumnCount(int extraColumnCount)
{
    m_extraColumnCount = extraColumnCount;
}

int GameOverviewModel::extraRowsCount() const
{
    return m_extraRowsCount;
}

void GameOverviewModel::setExtraRowsCount(int extraRowsCount)
{
    m_extraRowsCount = extraRowsCount;
}

QColor GameOverviewModel::colorFromPoints(int points)
{
    QColor color;
    if(points < 0) {
        color = QColor(237, 60, 46);
        for(;points > -10; --points) {
            color = color.darker(115 - points);
        }
    }
    else if(points > 0) {
        color = QColor(100, 211, 10);
        for(;points < 10; ++points) {
            color = color.darker(115 - points);
        }
    }
    return color;
}
