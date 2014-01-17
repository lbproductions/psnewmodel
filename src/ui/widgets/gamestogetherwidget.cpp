#include "gamestogetherwidget.h"
#include "ui_gamestogetherwidget.h"

#include <data/game.h>
#include <data/player.h>
#include <misc/tools.h>

GamesTogetherWidget::GamesTogetherWidget(QWidget *parent) :
    QTreeWidget(parent)
{
        //this->setStyleSheet("QHeaderView::section {background-color:transparent;}");

    this->setPalette(Tools::darkPalette(this));
    this->setColumnCount(5);

}

GamesTogetherWidget::~GamesTogetherWidget()
{
}

void GamesTogetherWidget::setData(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players)
{
    m_players = players;
    m_games = games;

    int rowCount = 0;
    for(int i = 0; i<players.size(); i++) {
        for(int j = i+1; j<players.size(); j++) {
            QTreeWidgetItem* item = new QTreeWidgetItem(this);
            item->setIcon(0, QIcon(Tools::playersColorPixmap(players.at(i), players.at(j))));
            QString playersString = players.at(i)->name() + " - " + players.at(j)->name();
            item->setText(0, playersString);

            m_items.insert(playersString, item);
            this->addTopLevelItem(item);

            rowCount++;
        }
    }

    update();

    foreach(QSharedPointer<Game> game, games) {
        connect(game.data(), SIGNAL(newRoundStarted()), this, SLOT(update()));
    }

    resizeColumnToContents(0);

    if(players.isEmpty())
        return;

    setMaximumHeight(rowCount*this->visualItemRect(this->topLevelItem(0)).height()+1);
    /*
    ui->treeWidgetGamesTogether->resizeColumnToContents(0);
    ui->treeWidgetGamesTogether->resizeColumnToContents(1);
    //ui->treeWidgetGamesTogether->header()->resizeSection(1, 25);
    ui->treeWidgetGamesTogether->resizeColumnToContents(2);
    ui->treeWidgetGamesTogether->resizeColumnToContents(3);
    //ui->treeWidgetGamesTogether->resizeColumnToContents(4);
    //ui->treeWidgetGamesTogether->resizeColumnToContents(5);
    //ui->treeWidgetGamesTogether->header()->resizeSection(0, 35);

    ui->treeWidgetGamesTogether->setMinimumWidth(250);
    ui->treeWidgetGamesTogether->sortByColumn(3);
    */
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
