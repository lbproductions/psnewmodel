#include "gameinformationmodel.h"

#include "gameoverviewmodel.h"

#include <data/game.h>
#include <data/player.h>
#include <misc/tools.h>

#include <misc/settings.h>

#include <QAction>
#include <QFont>
#include <QGuiApplication>

GameInformationModel::GameInformationModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_fontSize(13)
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
    connect(m_game.data(), SIGNAL(drinksChanged()), this, SIGNAL(layoutChanged()));
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

    if(role == Qt::FontRole) {
        QFont font = QGuiApplication::font();

        font.setPixelSize(m_fontSize);
        return QVariant::fromValue<QFont>(font);
    }

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
        else if(extraRow == GameOverviewModel::SchmeissereienRow) {
            return m_game->schmeissereiCount();
        }
        else if(extraRow == GameOverviewModel::NormalRoundRow) {
            return m_game->normalRoundCount();
        }
        else if(extraRow == GameOverviewModel::DrinksRow) {
            return m_game->liveDrinks().size();
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
            else if(GameSettings::instance().pointsDisplay() == GameSettings::Drinks) {
                return m_game->totalDrinkCount(player);
            }
        }
    }

    return QVariant();
}

QVariant GameInformationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
        return QVariant();

    if(role == ActionRole)
        return actionVariant(section);

    if(role == Qt::FontRole) {
        QFont font = QGuiApplication::font();

        font.setPixelSize(m_fontSize);
        return QVariant::fromValue<QFont>(font);
    }

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
        if(role == Qt::DecorationRole) {
            return QPixmap(":/gamewindow/schweinerei");
        }
        return tr("Schweinereien");
    }
    else if(extraRow == GameOverviewModel::SchmeissereienRow) {
        return tr("Schmeissereien");
    }
    else if(extraRow == GameOverviewModel::NormalRoundRow) {
        return tr("Normal round");
    }
    else if(extraRow == GameOverviewModel::DrinksRow) {
        return tr("Drinks");
    }
    else {
        QSharedPointer<Player> player;
        if(GameSettings::instance().playerSort() == GameSettings::SortByPosition) {
            player = m_game->players().at(section);
        }
        else if(GameSettings::instance().playerSort() == GameSettings::SortByPlacement) {
            player = m_game->playersSortedByPlacement().at(section);
        }

        if(role == Qt::DisplayRole) {
            return player->name();
        }
        else if(role == Qt::DecorationRole) {
            return player->color();
        }
        else if(role == GameInformationModel::PlayerRole) {
            return QVariant::fromValue<QSharedPointer<Player>>(player);
        }
    }

    return QVariant();
}

void GameInformationModel::triggerAction(int section)
{
    if(!m_actions.contains(section))
        return;

    QAction *action = m_actions.value(section);
    if(!action->isChecked())
        action->trigger();
}

void GameInformationModel::setHeaderAction(int section, QAction *action)
{
    m_actions.insert(section, action);

    connect(action, &QAction::changed, [=] {
        emit headerDataChanged(Qt::Horizontal, section, section);
    });
}

void GameInformationModel::removeHeaderAction(int section, QAction *action)
{
    m_actions.remove(section);
}

int GameInformationModel::fontSize() const
{
    return m_fontSize;
}

void GameInformationModel::setFontSize(int fontSize)
{
    beginResetModel();
    m_fontSize = fontSize;
    endResetModel();
}

void GameInformationModel::updateViews()
{
    beginResetModel();
    endResetModel();
}

QVariant GameInformationModel::actionVariant(int section) const
{
    return QVariant::fromValue<QAction *>(m_actions.value(section));
}
