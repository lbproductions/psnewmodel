#include "photoroundwidget.h"
#include "ui_photoroundwidget.h"

#include <QFileInfo>

#include <data/round.h>
#include <data/player.h>

#include "photoplayercardswidget.h"

PhotoRoundWidget::PhotoRoundWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoRoundWidget)
{
    ui->setupUi(this);
}

PhotoRoundWidget::~PhotoRoundWidget()
{
    delete ui;
}

void PhotoRoundWidget::setRound(QSharedPointer<Round> round, QStringList files)
{
    ui->labelRoundNumber->setText(tr("Round ") + QString::number(round->number() + 1));

    foreach(QSharedPointer<Player> player, round->rePlayers()) {
        PhotoPlayerCardsWidget* widget = new PhotoPlayerCardsWidget(this);
        widget->setPlayer(player);
        int playerID = Qp::primaryKey(player);
        foreach(QString file, files) {
            if(QFileInfo(file).baseName() == QString::number(playerID)) {
                widget->setImageFile(file);
                break;
            }
        }
        ui->verticalLayoutRePlayer->addWidget(widget);
    }

    foreach(QSharedPointer<Player> player, round->contraPlayers()) {
        PhotoPlayerCardsWidget* widget = new PhotoPlayerCardsWidget(this);
        widget->setPlayer(player);
        int playerID = Qp::primaryKey(player);
        foreach(QString file, files) {
            if(QFileInfo(file).baseName() == QString::number(playerID)) {
                widget->setImageFile(file);
                break;
            }
        }
        ui->verticalLayoutContraPlayer->addWidget(widget);
    }

    ui->labelPointResult->setText(QString::number(round->reGamePoints()) + " : " + QString::number(round->contraGamePoints()));

    if(round->winnerParty() == Round::Re) {
        ui->labelWinner->setText("Re, " + QString::number(round->points()) + tr(" points"));
    }
    else if(round->winnerParty() == Round::Contra) {
        ui->labelWinner->setText("Contra, " + QString::number(-round->points()) + tr(" points"));
    }

    ui->labelLength->setText(round->length().toString("mm:ss") + " mins");
    ui->labelType->setText(round->typeString());
}
