#include "gameoverviewmodel.h"

#include <data/game.h>
#include <data/round.h>
#include <data/player.h>

#include <QDebug>

const int GameOverviewModel::ExtraRowsCount(4);

GameOverviewModel::GameOverviewModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

QSharedPointer<Game> GameOverviewModel::game() const
{
    return m_game;
}

void GameOverviewModel::setGame(const QSharedPointer<Game> &game)
{
    beginResetModel();
    m_game = game;
    connect(m_game.data(), SIGNAL(newRoundStarted()), this, SIGNAL(layoutChanged()));
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

    return m_game->players().size() + ExtraRowsCount;
}

QVariant GameOverviewModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    int row = index.row();
    int column = index.column();
    int roundIndex = column;
    int playerIndex = row;
    int extraRow = row - m_game->players().size();

    if(roundIndex >= m_game->rounds().size())
        return QVariant();

    if(extraRow == HochzeitenRow) {
        QSharedPointer<Round> round = m_game->rounds().at(roundIndex);
        QSharedPointer<Player> player = round->hochzeitPlayer();

        if(player) {
            if(role == Qt::DecorationRole)
                return player->color();

            if(role == Qt::BackgroundColorRole)
                return colorFromPoints(round->points(player));

            if(role == MitspielerColorRole) {
                if(round->isSolo())
                    return QVariant();

                QSharedPointer<Player> mitspieler = round->re1Player();
                if(mitspieler == player)
                    mitspieler = round->re2Player();

                if(mitspieler)
                    return mitspieler->color();
            }
        }
    }
    else if(extraRow == TrumpfabgabenRow) {
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
    else if(extraRow == SoliRow) {
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
    if(extraRow == SchweinereienRow) {
        QSharedPointer<Round> round = m_game->rounds().at(roundIndex);
        QSharedPointer<Player> player = round->schweinereiPlayer();

        if(player) {
            if(role == Qt::DecorationRole)
                return player->color();

            if(role == Qt::BackgroundColorRole)
                return colorFromPoints(round->points(player));

        }
    }
    else if(playerIndex < m_game->players().size()
            && roundIndex < m_game->rounds().size()) {
        QSharedPointer<Player> player = m_game->players().at(playerIndex);
        QSharedPointer<Round> round = m_game->rounds().at(roundIndex);

        if(round->state() != Round::Finished)
            return QVariant();

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
            int roundIndex = section;
            if(roundIndex < m_game->rounds().size()) {
                if(m_game->players().size() > 0 &&
                        roundIndex % m_game->players().size() == 0)
                    return roundIndex + 1;
            }
        }
    }
//    else {
//        int row = section;
//        int playerIndex = row - m_extraRowsCount;

//        if(row < m_extraRowsCount) {
//            if(row == HochzeitenRow) {
//                return tr("Hochzeiten");
//            }
//            else if(row == TrumpfabgabenRow) {
//                return tr("Trumpfabgaben");
//            }
//            else if(row == SoliRow) {
//                return tr("Soli");
//            }
//        }
//        else {
//            if(role == Qt::DisplayRole) {
//                return m_game->players().at(playerIndex)->name();
//            }
//            else if(role == Qt::DecorationRole) {
//                return m_game->players().at(playerIndex)->color();
//            }
//        }
//    }

    return QVariant();
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
