#include "gamestogetherwidget.h"
#include "ui_gamestogetherwidget.h"

#include <data/game.h>
#include <data/player.h>
#include <misc/tools.h>

GamesTogetherWidget::GamesTogetherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamesTogetherWidget)
{
    ui->setupUi(this);

    this->setPalette(Tools::darkPalette(this));

    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButton);

    ui->treeWidgetGamesTogether->setPalette(Tools::darkPalette(ui->treeWidgetGamesTogether));
}

GamesTogetherWidget::~GamesTogetherWidget()
{
    delete ui;
}

void GamesTogetherWidget::setData(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players)
{
    for(int i = 0; i<players.size(); i++) {
        for(int j = i+1; j<players.size(); j++) {
            int gameCount = 0;
            int wins = 0;
            foreach(QSharedPointer<Game> game, games) {
                gameCount += game->roundsTogether(players.at(i), players.at(j));
                wins += game->winsTogether(players.at(i), players.at(j));
            }

            QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidgetGamesTogether);
            item->setIcon(0, QIcon(Tools::playersColorPixmap(players.at(i), players.at(j))));
            //item->setIcon(1, QIcon(players.at(j)->colorPixmap(10,10)));
            item->setText(0, players.at(i)->name() + " - " + players.at(j)->name());
            item->setData(1, Qt::DisplayRole, wins);
            item->setData(2, Qt::DisplayRole, gameCount);
            item->setData(3, Qt::DisplayRole, Tools::percentage(wins, gameCount));
            item->setTextAlignment(3, Qt::AlignRight);

            ui->treeWidgetGamesTogether->addTopLevelItem(item);
        }

    }

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

}

QTreeWidget *GamesTogetherWidget::gamesTogetherWidget()
{
    if(!ui)
        return 0;

    return ui->treeWidgetGamesTogether;
}

void GamesTogetherWidget::update()
{

}
