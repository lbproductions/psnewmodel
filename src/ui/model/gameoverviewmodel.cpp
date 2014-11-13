#include "gameoverviewmodel.h"

#include <data/game.h>
#include <data/round.h>
#include <data/player.h>
#include <data/schmeisserei.h>

#include <misc/settings.h>

#include <QFont>
#include <QDebug>
#include <QGuiApplication>

const int GameOverviewModel::ExtraRowsCount(7);

GameOverviewModel::GameOverviewModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_fontSize(13)
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
    connect(m_game.data(), SIGNAL(schmeissereiAdded()), this, SIGNAL(layoutChanged()));
    connect(m_game.data(), SIGNAL(drinksChanged()), this, SIGNAL(layoutChanged()));
    connect(&GameSettings::instance(), SIGNAL(playerSortChanged(PlayerSort)), this, SIGNAL(layoutChanged()));
    connect(&GameSettings::instance(), SIGNAL(tableDisplayChanged(TableDisplay)), this, SIGNAL(layoutChanged()));
    connect(&GameSettings::instance(), SIGNAL(showExtraRowsChanged(bool)), this, SIGNAL(layoutChanged()));
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

    // TODO: Find a way to paint the area outside the table with correct color
    if(m_game->totalRoundCount() > 50) {
        return m_game->totalRoundCount();
    }
    else{
        return 50;
    }
}

int GameOverviewModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    if(!m_game)
        return 0;

    if(GameSettings::instance().showExtraRows()) {
        return m_game->players().size() + ExtraRowsCount;
    }
    else{
        return m_game->players().size();
    }
}

QVariant GameOverviewModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::FontRole) {
        QFont font = QGuiApplication::font();

        font.setPixelSize(m_fontSize);
        return QVariant::fromValue<QFont>(font);
    }

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
            if(role == PflichtSoloRole) {
                return round->isPflicht();
            }
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
    if(extraRow == SchmeissereienRow) {
        QSharedPointer<Round> round = m_game->rounds().at(roundIndex);
        QList<QSharedPointer<Schmeisserei> > schmeissereien = round->schmeissereis();

        if(role == SchmeissereienRole) {
            QList<QColor> colors;
            foreach(auto schmeisserei, schmeissereien) {
                colors.append(schmeisserei->player()->color());
            }
            return QVariant::fromValue<QList<QColor> >(colors);
        }
    }
    else if(extraRow == NormalRoundRow) {
        QSharedPointer<Round> round = m_game->rounds().at(roundIndex);

        if(!round)
            return QVariant();

        QSharedPointer<Player> re1Player = round->re1Player();
        QSharedPointer<Player> re2Player = round->re2Player();

        if(!re1Player || !re2Player || round->trumpfabgabePlayer() || round->hochzeitPlayer()) {
            return QVariant();
        }

        if(role == Qt::DecorationRole)
            return re1Player->color();

        if(role == Qt::BackgroundColorRole)
            return colorFromPoints(round->points(re1Player));

        if(role == MitspielerColorRole) {
            return re2Player->color();
        }

    }
    else if(extraRow == DrinksRow) {
        QSharedPointer<Round> round = m_game->rounds().at(roundIndex);

        if(!round)
            return QVariant();

        if(role == Qt::DisplayRole) {
            int count =  round->liveDrinks().size();
            if(count > 0) {
                return count;
            }

            return QVariant();
        }
    }
    else if(playerIndex < m_game->players().size()
            && roundIndex < m_game->rounds().size()) {
        QSharedPointer<Player> player;
        if(GameSettings::instance().playerSort() == GameSettings::SortByPosition) {
            player = m_game->players().at(playerIndex);
        }
        else if(GameSettings::instance().playerSort() == GameSettings::SortByPlacement) {
            player = m_game->playersSortedByPlacement().at(playerIndex);
        }
        QSharedPointer<Round> round = m_game->rounds().at(roundIndex);

        if(round->state() != Round::Finished)
            return QVariant();

        if(GameSettings::instance().tableDisplay() == GameSettings::RoundPoints && round->playingPlayers().contains(player)) {
            if(role == Qt::DisplayRole)
                return round->points(player);

            if(role == Qt::BackgroundColorRole)
                return colorFromPoints(round->points(player));
            // TODO: could be better in performance
            if(role == IsReRole) {
                return round->rePlayers().contains(player);
            }
        }
        else if(GameSettings::instance().tableDisplay() == GameSettings::TableTotalPoints) {
            if(role == Qt::DisplayRole) {
                return round->totalPoints(player);
            }
            if(role == Qt::BackgroundColorRole) {
                return colorFromPoints(round->totalPoints(player));
            }
        }
        else if(GameSettings::instance().tableDisplay() == GameSettings::DeficitToLeader) {
            if(role == Qt::DisplayRole) {
                return round->pointsToLeader(player);
            }
            if(role == Qt::BackgroundColorRole) {
                return colorFromPoints(-round->pointsToLeader(player));
            }
        }
    }

    return QVariant();
}

QVariant GameOverviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal) {

        if(role == Qt::FontRole) {
            QFont font = QGuiApplication::font();

            font.setPixelSize(m_fontSize);
            return QVariant::fromValue<QFont>(font);
        }

        int roundIndex = section;
        int count = m_game->rounds().size();
        int total = m_game->totalRoundCount();
        int roundsLeft = total - count + 1;
        int playerSize = m_game->players().size();

        if(role == Qt::DisplayRole) {
            if(roundIndex >= total)
                return QVariant();

            if(m_game->state() == Game::Finished) {
                return QVariant();
            }

            if(roundIndex < count && playerSize > 0 && roundIndex % playerSize == 0) {
                return roundIndex + 1;
            }
            else if(roundsLeft > 0) {
                if(roundIndex == total - 2)
                    return tr("%1").arg(roundsLeft);
                if(roundIndex == total - 1)
                    return tr(" left");
            }
        }

        if(role == AlignmentRole) {
            if(roundsLeft > 0) {
                if(roundIndex == total - 2)
                    return Qt::AlignRight;
                if(roundIndex == total - 1)
                    return Qt::AlignLeft;
            }

            return Qt::AlignCenter;
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
int GameOverviewModel::fontSize() const
{
    return m_fontSize;
}

void GameOverviewModel::setFontSize(int fontSize)
{
    beginResetModel();
    m_fontSize = fontSize;
    endResetModel();
}

void GameOverviewModel::updateViews()
{
    beginResetModel();
    endResetModel();
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
