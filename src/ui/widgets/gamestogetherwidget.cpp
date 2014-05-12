#include "gamestogetherwidget.h"

#include <data/game.h>
#include <data/league.h>
#include <data/player.h>
#include <misc/tools.h>

#include  <QHeaderView>

GamesTogetherWidget::GamesTogetherWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    this->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());

    this->setPalette(Tools::darkPalette(this));
    this->setColumnCount(4);
    this->setHeaderLabels(QStringList() << tr("Players") << tr("Wins") << tr("Rounds") << tr("WinPercentage"));
    this->setSortingEnabled(true);

    setAttribute(Qt::WA_MacShowFocusRect, false);
}

GamesTogetherWidget::~GamesTogetherWidget()
{
}

void GamesTogetherWidget::setGames(QList<QSharedPointer<Game> > games)
{
    QList<QSharedPointer<Player> > players;
    foreach(QSharedPointer<Game> game, games) {
        foreach(QSharedPointer<Player> player, game->players()) {
            if(!players.contains(player)) {
                players.append(player);
            }
        }
    }

    setGames(games, players);
}

void GamesTogetherWidget::setGames(QList<QSharedPointer<Game> > list, QList<QSharedPointer<Player> > players)
{
    m_games = list;
    m_players = players;

    foreach(QSharedPointer<Game> game, list) {
        connect(game.data(), SIGNAL(newRoundStarted()), this, SLOT(update()));
    }

    int rowCount = 0;
    for(int i = 0; i<m_players.size(); i++) {
        for(int j = i+1; j<m_players.size(); j++) {
            QTreeWidgetItem* item = new QTreeWidgetItem(this);
            item->setIcon(0, QIcon(Tools::playersColorPixmap(m_players.at(i), m_players.at(j))));
            QString playersString = m_players.at(i)->name() + " - " + m_players.at(j)->name();
            item->setText(0, playersString);

            m_items.insert(playersString, item);
            this->addTopLevelItem(item);

            rowCount++;
        }
    }

    update();

    resizeColumnToContents(0);

    setMaximumHeight(rowCount*this->visualItemRect(this->topLevelItem(0)).height()+1);
}

void GamesTogetherWidget::setLeague(QSharedPointer<League> league)
{
    setGames(league->calculatedGames(), league->players());
}

void GamesTogetherWidget::update()
{
    for(int i = 0; i<m_players.size(); i++) {
        for(int j = i+1; j<m_players.size(); j++) {

            int gameCount = 0;
            int wins = 0;
            foreach(QSharedPointer<Game> game, m_games) {
                gameCount += game->roundsTogether(m_players.at(i), m_players.at(j));
                wins += game->winsTogether(m_players.at(i), m_players.at(j));
            }

            QString playersString = m_players.at(i)->name() + " - " + m_players.at(j)->name();
            QTreeWidgetItem* item = m_items.value(playersString);
            item->setData(1, Qt::DisplayRole, wins);
            item->setData(2, Qt::DisplayRole, gameCount);
            item->setData(3, Qt::DisplayRole, Tools::percentage(wins, gameCount));
            item->setTextAlignment(3, Qt::AlignRight);
        }
    }


    resizeColumnToContents(1);
    resizeColumnToContents(2);
    resizeColumnToContents(3);
    sortByColumn(3);
}
