#include "schmeissereidialog.h"
#include "ui_schmeissereidialog.h"

#include <data/round.h>
#include <data/player.h>
#include <data/schmeisserei.h>

#include <QPushButton>

SchmeissereiDialog::SchmeissereiDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SchmeissereiWidget)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &SchmeissereiDialog::save);
    connect(ui->buttonBox, &QDialogButtonBox::rejected,
            this, &SchmeissereiDialog::reject);
    connect(ui->comboBoxPlayer, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkContents()));

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
    if(!round)
        return;

    m_round = round;
    ui->comboBoxPlayer->clear();
    ui->comboBoxPlayer->addPlayers(round->playingPlayers());
    ui->comboBoxType->addItems(Schmeisserei::typeStrings());
}

void SchmeissereiDialog::save()
{
    if(!m_round)
        return;

    QSharedPointer<Schmeisserei> schmeisserei = Qp::create<Schmeisserei>();
    schmeisserei->setPlayer(ui->comboBoxPlayer->currentPlayer());
    schmeisserei->setType(Schmeisserei::typeFromString(ui->comboBoxType->currentText()));
    m_round->addSchmeisserei(schmeisserei);
    Qp::update(schmeisserei);
    Qp::update(m_round);
    accept();
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
