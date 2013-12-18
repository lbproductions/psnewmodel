#include "solooverviewwidget.h"
#include "ui_solooverviewwidget.h"

#include <data/game.h>
#include <data/player.h>

#include <misc/tools.h>

SoloOverviewWidget::SoloOverviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SoloOverviewWidget)
{
    ui->setupUi(this);

    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonSoloPlayer);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonSoloType);

    ui->treeWidgetSoloType->setPalette(Tools::darkPalette(ui->treeWidgetSoloType));
    ui->treeWidgetSoloPlayer->setPalette(Tools::darkPalette(ui->treeWidgetSoloPlayer));
    this->setPalette(Tools::darkPalette(this));

    ui->stackedWidget->setContentsMargins(0,0,0,0);
    ui->stackedWidget->layout()->setContentsMargins(0,0,0,0);
    ui->treeWidgetSoloPlayer->setContentsMargins(0,0,0,0);
    ui->treeWidgetSoloType->setContentsMargins(0,0,0,0);

    ui->buttonGroup->setId(ui->pushButtonSoloType, 1);
    ui->buttonGroup->setId(ui->pushButtonSoloPlayer, 0);
    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
}

SoloOverviewWidget::~SoloOverviewWidget()
{
    delete ui;
}

void SoloOverviewWidget::setGames(QList<QSharedPointer<Game> > games)
{
    m_games = games;

    createCountHash();

    foreach(Round::SoloType type, m_counts.keys()) {
        int count = 0;

        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidgetSoloType);
        item->setText(1, Round::soloTypeStringFromType(type));
        item->setTextAlignment(2, Qt::AlignRight);

        foreach(QSharedPointer<Player> player, m_counts.value(type).keys()) {
            count += m_counts.value(type).value(player);

            QTreeWidgetItem* subItem = new QTreeWidgetItem(item);
            subItem->setText(1, player->name());
            subItem->setData(2, Qt::DisplayRole, m_counts.value(type).value(player));
            subItem->setTextAlignment(2, Qt::AlignRight);
            item->addChild(subItem);
        }
        item->setData(2, Qt::DisplayRole, count);

        ui->treeWidgetSoloType->addTopLevelItem(item);
    }
    ui->treeWidgetSoloType->resizeColumnToContents(0);
    ui->treeWidgetSoloType->resizeColumnToContents(1);
    ui->treeWidgetSoloType->resizeColumnToContents(2);
    ui->treeWidgetSoloType->sortByColumn(2);

    foreach(QSharedPointer<Player> player, m_players) {
        int totalCount = 0;

        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidgetSoloPlayer);
        item->setText(1, player->name());
        item->setIcon(0, QIcon(player->colorPixmap()));

        foreach(Round::SoloType type, m_counts.keys()) {
            totalCount += m_counts.value(type).value(player);

            QTreeWidgetItem* subItem = new QTreeWidgetItem(item);
            subItem->setText(1, Round::soloTypeStringFromType(type));
            subItem->setData(2, Qt::DisplayRole, m_counts.value(type).value(player));
            subItem->setTextAlignment(2, Qt::AlignRight);
            item->addChild(subItem);
        }


        item->setData(2, Qt::DisplayRole, totalCount);
        item->setTextAlignment(2, Qt::AlignRight);

        ui->treeWidgetSoloPlayer->addTopLevelItem(item);
    }

    ui->treeWidgetSoloPlayer->resizeColumnToContents(0);
    ui->treeWidgetSoloPlayer->header()->resizeSection(1,175);
    ui->treeWidgetSoloPlayer->resizeColumnToContents(2);
    ui->treeWidgetSoloPlayer->sortByColumn(2);

    ui->treeWidgetSoloPlayer->setMinimumWidth(250);
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

