#include "solooverviewwidget.h"
#include "ui_solooverviewwidget.h"

#include <QLabel>

#include <data/game.h>
#include <data/player.h>
#include <data/league.h>
#include <data/playerstatistics.h>

#include <misc/tools.h>

SoloOverviewWidget::SoloOverviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SoloOverviewWidget)
{
    ui->setupUi(this);

    this->setPalette(Tools::darkPalette(this));
    ui->treeWidget->setPalette(Tools::darkPalette(ui->treeWidget));
    ui->treeWidget->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());

    ui->treeWidget->setAttribute(Qt::WA_MacShowFocusRect, false);
}

SoloOverviewWidget::~SoloOverviewWidget()
{
    delete ui;
}

void SoloOverviewWidget::setGames(QList<QSharedPointer<Game> > games)
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

void SoloOverviewWidget::setGames(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players)
{
    m_players = players;

    foreach(QSharedPointer<Player> player, players) {
        QSharedPointer<PlayerStatistics> stats = QSharedPointer<PlayerStatistics>(new PlayerStatistics(this));
        stats->setPlayer(player.data());
        QList<QSharedPointer<Game> > playerGames;
        foreach(QSharedPointer<Game> game, games) {
            if(game->players().contains(player)) {
                playerGames.append(game);
            }
        }
        stats->setGames(playerGames);

        m_stats.insert(player, stats);
    }

    init();
}

void SoloOverviewWidget::setLeague(QSharedPointer<League> league)
{
    m_players = league->players();

    foreach(QSharedPointer<Player> player, league->players()) {
        m_stats.insert(player, league->playerStats(player));
    }
    init();
}

void SoloOverviewWidget::init()
{
    int totalSoli = totalCount();
    if(totalSoli == 0) {
        return;
    }

    QList<QColor> colors = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta, Qt::black, Qt::white};

    foreach(Round::SoloType type, Round::soloTypes()) {
        int count = totalTypeCount(type);
        double value = (double)count / (double)totalSoli * 100.0;
        if(value <= 0) {
            continue;
        }
        ui->pieWidget->addValue(Round::soloTypeStringFromType(type) + QString(" [%1]").arg(count), value, colors.at(type-2));
    }


    QStringList labels = {tr("Player") , tr("TotalSoli") , tr("TotalWins") , tr("RelWinCount") , tr("Pflicht") , tr("PflichtWin") , tr("PflichtRel")};
    /*
    QString whiteLabelStyleSheet = "QLabel{color:white;}";

    int row = 1;
    int totalCount = 0;
    foreach(Round::SoloType type, Round::soloTypes()) {
        QLabel* name = new QLabel(Round::soloTypeStringFromType(type) + ":", this);
        name->setStyleSheet(whiteLabelStyleSheet);
        ui->gridLayout->addWidget(name, row, 0);
        int typeCount = totalTypeCount(type);
        totalCount += typeCount;
        QLabel* value = new QLabel(QString::number(typeCount));
        value->setStyleSheet(whiteLabelStyleSheet);
        ui->gridLayout->addWidget(value, row, 1);
        row++;

        labels.append(Round::soloTypeStringFromType(type));
    }

    QLabel* totalName = new QLabel(tr("Total count:"), this);
    totalName->setStyleSheet(whiteLabelStyleSheet);
    ui->gridLayout->addWidget(totalName, 0, 0);
    QLabel* totalValue = new QLabel(QString::number(totalCount), this);
    totalValue->setStyleSheet(whiteLabelStyleSheet);
    ui->gridLayout->addWidget(totalValue, 0, 1);
    */

    foreach(QSharedPointer<Player> player, m_players) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, player->name());
        item->setIcon(0, QIcon(player->colorPixmap()));
        int rounds = m_stats.value(player)->soloRounds().size();
        int wins = m_stats.value(player)->soloWinRounds().size();
        item->setData(1, Qt::DisplayRole, rounds);
        item->setData(2, Qt::DisplayRole, wins);
        double ratio = 0;
        if(rounds > 0) {
            ratio = (double)wins * 100 / (double)rounds;
        }
        item->setData(3, Qt::DisplayRole, ratio);
        rounds = m_stats.value(player)->soloPflichtRounds().size();
        wins = m_stats.value(player)->soloPflichtWinRounds().size();
        item->setData(4, Qt::DisplayRole, rounds);
        item->setData(5, Qt::DisplayRole, wins);
        ratio = 0;
        if(rounds > 0) {
            ratio = (double)wins * 100 / (double)rounds;
        }
        item->setData(6, Qt::DisplayRole, ratio);
        int column = 7;
        foreach(Round::SoloType type, Round::soloTypes()) {
            item->setData(column, Qt::DisplayRole, QString::number(m_stats.value(player)->soloWinRounds(type).size()) + " / " + QString::number(m_stats.value(player)->soloRounds(type).size()));
            column++;
        }
    }

    foreach(Round::SoloType type, Round::soloTypes()) {
        labels.append(Round::soloTypeStringFromType(type));
    }

    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->setColumnCount(labels.size());
    ui->treeWidget->setHeaderLabels(labels);

}

int SoloOverviewWidget::totalTypeCount(Round::SoloType type)
{
    int count = 0;
    foreach(QSharedPointer<Player> player, m_players) {
        count += m_stats.value(player)->soloRounds(type).size();
    }
    return count;
}

int SoloOverviewWidget::totalCount()
{
    int result = 0;

    foreach(Round::SoloType type, Round::soloTypes()) {
        result += totalTypeCount(type);
    }

    return result;
}


