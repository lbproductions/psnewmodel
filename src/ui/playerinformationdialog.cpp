#include "playerinformationdialog.h"
#include "ui_playerinformationdialog.h"

#include <data/player.h>

PlayerInformationDialog::PlayerInformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerInformationDialog)
{
    ui->setupUi(this);

    ui->color->setType(ImageWell::ColorWell);
}

PlayerInformationDialog::~PlayerInformationDialog()
{
    delete ui;
}

QSharedPointer<Player> PlayerInformationDialog::player() const
{
    return m_player;
}

void PlayerInformationDialog::setPlayer(const QSharedPointer<Player> &player)
{
    m_player = player;
    setWindowTitle(player->name());
    ui->lineEditName->setText(player->name());
    if(player->height() > 0)
        ui->lineEditHeight->setText(QString::number(player->height()));
    if(player->weight() > 0)
    ui->lineEditWeight->setText(QString::number(player->weight()));
    ui->image->setPixmap(player->avatar());
    ui->color->setColor(player->color());
    if(player->gender() == Player::Male)
        ui->radioButtonMale->setChecked(true);
    else if(player->gender() == Player::Female)
        ui->radioButtonFemale->setChecked(true);
}

void PlayerInformationDialog::accept()
{
    if(!m_player)
        return;

    m_player->setName(ui->lineEditName->text());
    m_player->setHeight(ui->lineEditHeight->text().toInt());
    m_player->setWeight(ui->lineEditWeight->text().toInt());
    m_player->setAvatar(ui->image->pixmap());
    m_player->setColor(ui->color->color());

    if(ui->radioButtonMale->isChecked())
        m_player->setGender(Player::Male);
    else if(ui->radioButtonFemale->isChecked())
        m_player->setGender(Player::Female);

    Qp::update(m_player);
    QDialog::accept();
}
