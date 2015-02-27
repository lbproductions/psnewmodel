#include "gamestogetherwidget.h"

#include <data/game.h>
#include <data/league.h>
#include <data/player.h>
#include <misc/tools.h>

#include  <QHeaderView>

GamesTogetherWidget::GamesTogetherWidget(QWidget *parent) :
    StatsTreeWidget(parent)
{
    this->setColumnCount(6);
    this->setHeaderLabels(QStringList() << tr("Players") << tr("Games together")<< tr("Wins") << tr("Wins/Game") << tr("Rounds") << tr("Rounds/Game") << tr("WinPercentage"));
}

GamesTogetherWidget::~GamesTogetherWidget()
{
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
}

void GamesTogetherWidget::update()
{
    for(int i = 0; i<m_players.size(); i++) {
        for(int j = i+1; j<m_players.size(); j++) {

            int roundCount = 0;
            int wins = 0;
            int gamesTogether = 0;
            foreach(QSharedPointer<Game> game, m_games) {
                if(game->players().contains(m_players.at(i)) && game->players().contains(m_players.at(j))) {
                    roundCount += game->roundsTogether(m_players.at(i), m_players.at(j));
                    wins += game->winsTogether(m_players.at(i), m_players.at(j));
                    gamesTogether++;
                }
            }

            if(gamesTogether == 0) {
                continue;
            }

            QString playersString = m_players.at(i)->name() + " - " + m_players.at(j)->name();
            QTreeWidgetItem* item = m_items.value(playersString);
            item->setData(1, Qt::DisplayRole, gamesTogether);
            item->setData(2, Qt::DisplayRole, wins);
            item->setData(3, Qt::DisplayRole, (double)wins / (double)gamesTogether);
            item->setData(4, Qt::DisplayRole, roundCount);
            item->setData(5, Qt::DisplayRole, (double)roundCount / (double)gamesTogether);
            item->setData(6, Qt::DisplayRole, Tools::percentage(wins, roundCount));
            item->setTextAlignment(6, Qt::AlignRight);
        }
    }


    setColumnsWidthToContent();
    highlightMax(Qt::DescendingOrder, QColor(0,123,0));

    sortByColumn(5);
}
