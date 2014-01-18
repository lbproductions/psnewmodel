#include "solooverviewwidget.h"
#include "ui_solooverviewwidget.h"

#include <QLabel>

#include <data/game.h>
#include <data/player.h>

#include <misc/tools.h>

SoloOverviewWidget::SoloOverviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SoloOverviewWidget)
{
    ui->setupUi(this);

    this->setPalette(Tools::darkPalette(this));
    ui->treeWidget->header()->setStyleSheet(Tools::darkHeaderViewStyleSheet());
}

SoloOverviewWidget::~SoloOverviewWidget()
{
    delete ui;
}

void SoloOverviewWidget::setGames(QList<QSharedPointer<Game> > games)
{
    m_games = games;

    createCountHash();

    QStringList labels;
    labels << tr("Player") << tr("TotalSoli");

    QString whiteLabelStyleSheet = "QLabel{color:white;}";

    int row = 1;
    int totalCount = 0;
    foreach(Round::SoloType type, m_counts.keys()) {
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

    ui->treeWidget->setColumnCount(labels.size());
    ui->treeWidget->setHeaderLabels(labels);

    foreach(QSharedPointer<Player> player, m_players) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, player->name());
        item->setIcon(0, QIcon(player->colorPixmap()));
        item->setData(1, Qt::DisplayRole, totalPlayerCount(player));
        int column = 2;
        foreach(Round::SoloType type, m_counts.keys()) {
            item->setData(column, Qt::DisplayRole, m_counts.value(type).value(player));
            column++;
        }
    }
}

void SoloOverviewWidget::createCountHash()
{
    m_counts.clear();

    foreach(QSharedPointer<Game> game, m_games) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(round->isSolo()) {
                if(m_counts.value(round->soloType()).isEmpty()) {
                    QHash<QSharedPointer<Player>,int > hash;
                    hash.insert(round->soloPlayer(), 1);
                    m_counts.insert(round->soloType(), hash);
                }
                else{
                    QHash<QSharedPointer<Player>,int > hash = m_counts.value(round->soloType());
                    hash.insert(round->soloPlayer(), hash.value(round->soloPlayer()) + 1);
                    m_counts.insert(round->soloType(), hash);
                }
                if(!m_players.contains(round->soloPlayer())) {
                    m_players.append(round->soloPlayer());
                }
            }
        }
    }
}

int SoloOverviewWidget::totalTypeCount(Round::SoloType type)
{
    int count = 0;
    QHash<QSharedPointer<Player>,int> hash = m_counts.value(type);
    foreach(QSharedPointer<Player> player, hash.keys()) {
        count += hash.value(player);
    }
    return count;
}

int SoloOverviewWidget::totalPlayerCount(QSharedPointer<Player> player)
{
    int count = 0;
    for(int i = 0; i<m_counts.keys().size(); i++) {
        count += m_counts.value(m_counts.keys().at(i)).value(player);
    }
    return count;
}

