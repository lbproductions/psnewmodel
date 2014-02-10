#include "schmeissereidialog.h"
#include "ui_schmeissereidialog.h"

#include <data/round.h>
#include <data/player.h>
#include <data/schmeisserei.h>

#include <QPushButton>
#include <functional>


SchmeissereiDialog::SchmeissereiDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SchmeissereiWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &SchmeissereiDialog::save);
    connect(ui->buttonBox, &QDialogButtonBox::rejected,
            this, &SchmeissereiDialog::close);

    void (QComboBox::*currentIndexChangedSignal)(int) = &QComboBox::currentIndexChanged;
    connect(ui->comboBoxPlayer, currentIndexChangedSignal,
            this, &SchmeissereiDialog::checkContents);

    checkContents();
}


SchmeissereiDialog::~SchmeissereiDialog()
{
    delete ui;
}

QSharedPointer<Game> SchmeissereiDialog::game() const
{
    return m_game;
}

void SchmeissereiDialog::setGame(const QSharedPointer<Game> &game)
{
    m_game = game;
    int roundCount = game->rounds().size();
    ui->spinBoxRoundNumber->setMinimum(1);
    ui->spinBoxRoundNumber->setMaximum(roundCount);
    ui->spinBoxRoundNumber->setValue(roundCount);
}


QSharedPointer<Round> SchmeissereiDialog::round() const
{
    return m_round;
}

void SchmeissereiDialog::setRound(const QSharedPointer<Round> &round)
{
    if(!round || m_round == round)
        return;

    m_round = round;
    ui->comboBoxPlayer->clear();
    ui->comboBoxPlayer->addPlayers(round->playingPlayers());

    if(ui->comboBoxType->count() == 0)
        ui->comboBoxType->addItems(Schmeisserei::typeStrings());
}

void SchmeissereiDialog::save()
{
    if(!m_round)
        return;

    QSharedPointer<Schmeisserei> schmeisserei = Qp::create<Schmeisserei>();
    schmeisserei->setPlayer(ui->comboBoxPlayer->currentPlayer());
    Schmeisserei::Type type = Schmeisserei::typeFromString(ui->comboBoxType->currentText());
    schmeisserei->setType(type);

    if(type == Schmeisserei::MoreThan4Kings) {
        schmeisserei->setKingsCount(ui->spinBoxCount->value());
    }
    else if(type == Schmeisserei::NoTrumpfabgabeTaker
            || type == Schmeisserei::LessThan3Trumpf) {
        schmeisserei->setTrumpfCount(ui->spinBoxCount->value());
    }
    else if(type == Schmeisserei::MoreThan90Points) {
        schmeisserei->setPoints(ui->spinBoxCount->value());
    }

    m_round->addSchmeisserei(schmeisserei);
    Qp::update(schmeisserei);
    Qp::update(m_round);
    close();
}

void SchmeissereiDialog::checkContents()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(ui->comboBoxPlayer->currentPlayer());
}

void SchmeissereiDialog::on_spinBoxRoundNumber_valueChanged(int value)
{
    if(!m_game)
        return;

    int number = value - 1;
    Q_ASSERT(number >= 0);
    Q_ASSERT(number < m_game->rounds().size());

    QSharedPointer<Round> round = m_game->rounds().at(number);
    setRound(round);
}

void SchmeissereiDialog::on_comboBoxType_currentIndexChanged(const QString &text)
{
    Schmeisserei::Type type = Schmeisserei::typeFromString(text);

    ui->spinBoxCount->setEnabled(true);
    if(type == Schmeisserei::MoreThan4Kings) {
        ui->labelCount->setText(tr("Kings:"));
        ui->spinBoxCount->setMinimum(5);
        ui->spinBoxCount->setMaximum(8);
    }
    else if(type == Schmeisserei::NoTrumpfabgabeTaker) {
        ui->labelCount->setText(tr("Trümpfe:"));
        ui->spinBoxCount->setMinimum(1);
        ui->spinBoxCount->setMaximum(3);
    }
    else if(type == Schmeisserei::LessThan3Trumpf) {
        ui->labelCount->setText(tr("Trümpfe:"));
        ui->spinBoxCount->setMinimum(1);
        ui->spinBoxCount->setMaximum(2);

    }
    else if(type == Schmeisserei::MoreThan90Points) {
        ui->labelCount->setText(tr("Points:"));
        ui->spinBoxCount->setMinimum(90);
        ui->spinBoxCount->setMaximum(110);
    }
    else {
        ui->spinBoxCount->setEnabled(false);
    }
}
