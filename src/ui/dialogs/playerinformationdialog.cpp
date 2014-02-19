#include "playerinformationdialog.h"
#include "ui_playerinformationdialog.h"

#include <data/player.h>
#include <ui/model/playerslistmodel.h>

#include <QPushButton>
#include <QMenuBar>

PlayerInformationDialog::PlayerInformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerInformationDialog),
    m_currentIndex(-1),
    m_model(nullptr)
{
    ui->setupUi(this);

    ui->color->setType(ImageWell::ColorWell);

    m_pushButtonPrevPlayer = ui->buttonBox->addButton(tr("Previous"), QDialogButtonBox::ApplyRole);
    m_pushButtonNextPlayer = ui->buttonBox->addButton(tr("Next"), QDialogButtonBox::ApplyRole);
    connect(m_pushButtonPrevPlayer, &QPushButton::clicked,
            this, &PlayerInformationDialog::previousPlayer);
    connect(m_pushButtonNextPlayer, &QPushButton::clicked,
            this, &PlayerInformationDialog::nextPlayer);

    addAction(ui->actionClose);
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
    m_pushButtonNextPlayer->setEnabled(m_model && m_currentIndex + 1 < m_model->rowCount());
    m_pushButtonPrevPlayer->setEnabled(m_model && m_currentIndex - 1 >= 0);

    setWindowTitle(player->name());
    ui->lineEditName->setText(player->name());
    if(player->height() > 0)
        ui->lineEditHeight->setText(QString::number(player->height()));
    else
        ui->lineEditHeight->setText(QString());

    if(player->weight() > 0)
        ui->lineEditWeight->setText(QString::number(player->weight()));
    else
        ui->lineEditWeight->setText(QString());

    ui->image->setPixmap(player->avatar());
    ui->color->setColor(player->color());
    if(player->gender() == Player::Male)
        ui->radioButtonMale->setChecked(true);
    else if(player->gender() == Player::Female)
        ui->radioButtonFemale->setChecked(true);
}

void PlayerInformationDialog::setPlayerFromModel(QpSortFilterProxyObjectModel<Player> *model, int index)
{
    m_model = model;
    m_currentIndex = index;

    QSharedPointer<Player> player = model->objectByIndex(model->index(m_currentIndex));
    setPlayer(player);
}

void PlayerInformationDialog::accept()
{
    saveCurrentPlayer();
    QDialog::accept();
}

void PlayerInformationDialog::nextPlayer()
{
    if(m_currentIndex + 1 >=  m_model->rowCount())
        return;

    saveCurrentPlayer();
    ++m_currentIndex;
    QSharedPointer<Player> player = m_model->objectByIndex(m_model->index(m_currentIndex));
    setPlayer(player);
}

void PlayerInformationDialog::previousPlayer()
{
    if(m_currentIndex - 1 < 0)
        return;

    saveCurrentPlayer();

    --m_currentIndex;
    QSharedPointer<Player> player = m_model->objectByIndex(m_model->index(m_currentIndex));
    setPlayer(player);
}

void PlayerInformationDialog::saveCurrentPlayer()
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
}

void PlayerInformationDialog::on_actionClose_triggered()
{
    close();
}
