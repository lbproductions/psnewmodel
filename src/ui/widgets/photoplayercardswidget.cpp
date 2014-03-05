#include "photoplayercardswidget.h"
#include "ui_photoplayercardswidget.h"

#include <data/player.h>

PhotoPlayerCardsWidget::PhotoPlayerCardsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoPlayerCardsWidget)
{
    ui->setupUi(this);
}

PhotoPlayerCardsWidget::~PhotoPlayerCardsWidget()
{
    delete ui;
}

void PhotoPlayerCardsWidget::setPlayer(QSharedPointer<Player> player)
{
    m_player = player;

    ui->labelPlayerName->setText(player->name());
}

void PhotoPlayerCardsWidget::setImageFile(QString file)
{
    ui->imageLabel->setStyleSheet("");
    ui->imageLabel->setImage(QImage(file));
}
