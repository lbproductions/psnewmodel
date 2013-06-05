#include "newrounddialog.h"
#include "ui_newrounddialog.h"

#include <data/round.h>
#include <data/game.h>
#include <misc/tools.h>

#include <QFile>

NewRoundDialog::NewRoundDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewRoundDialog),
    m_doppelkopfRound(0)
{
    ui->setupUi(this);

//    setWindowFlags(Qt::FramelessWindowHint);

    ui->buttonGroup->setId(ui->pushButtonNormal, 0);
    ui->buttonGroup->setId(ui->pushButtonHochzeit, 1);
    ui->buttonGroup->setId(ui->pushButtonSolo, 2);
    ui->buttonGroup->setId(ui->pushButtonTrumpfabgabe, 3);

    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonNormal);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonHochzeit);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonSolo);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", ui->pushButtonTrumpfabgabe,
                                     "QWidget { margin-right: -2px; }");

    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    connect(ui->comboBoxNormalRe1, SIGNAL(currentIndexChanged(int)), this, SLOT(checkNormalRoundContents()));
    connect(ui->comboBoxNormalRe2, SIGNAL(currentIndexChanged(int)), this, SLOT(checkNormalRoundContents()));
    connect(ui->spinBoxNormalPoints,SIGNAL(valueChanged(int)), this, SLOT(checkNormalRoundContents()));

    connect(ui->comboBoxHochzeitHochzeit, SIGNAL(currentIndexChanged(int)), this, SLOT(checkHochzeitRoundContents()));
    connect(ui->comboBoxHochzeitFellow, SIGNAL(currentIndexChanged(int)), this, SLOT(checkHochzeitRoundContents()));
    connect(ui->spinBoxHochzeitPoints,SIGNAL(valueChanged(int)),this, SLOT(checkHochzeitRoundContents()));

    connect(ui->comboBoxSoloPlayer, SIGNAL(currentIndexChanged(int)), this, SLOT(checkSoloRoundContents()));
    connect(ui->comboBoxSoloType, SIGNAL(currentIndexChanged(int)), this, SLOT(checkSoloRoundContents()));
    connect(ui->spinBoxSoloPoints,SIGNAL(valueChanged(int)), this, SLOT(checkSoloRoundContents()));

    connect(ui->comboBoxTrumpfabgabePlayer, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTrumpfabgabeRoundContents()));
    connect(ui->comboBoxTrumpfabgabeAccept, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTrumpfabgabeRoundContents()));
    connect(ui->spinBoxTrumpfabgabePoints,SIGNAL(valueChanged(int)),this, SLOT(checkTrumpfabgabeRoundContents()));

    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(setCurrentPage(int)));

    connect(ui->buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked()), this, SLOT(save()));
}

NewRoundDialog::~NewRoundDialog()
{
    delete ui;
}

void NewRoundDialog::setDoppelkopfRound(QSharedPointer<Round> round)
{
    if(!round || m_doppelkopfRound == round)
        return;

    QSharedPointer<Game> game = round->game();

    m_doppelkopfRound = round;
    ui->comboBoxNormalRe1->addPlayers(round->playingPlayers());
    ui->comboBoxNormalRe1->setCurrentPlayer(round->re1Player());
    ui->comboBoxNormalRe2->addPlayers(round->playingPlayers());
    ui->comboBoxNormalRe2->setCurrentPlayer(round->re2Player());
    ui->comboBoxNormalSchweine->addPlayers(round->playingPlayers());
    ui->comboBoxNormalSchweine->setCurrentPlayer(round->schweinereiPlayer());

    ui->comboBoxHochzeitHochzeit->addPlayers(round->playingPlayers());
    ui->comboBoxHochzeitHochzeit->setCurrentPlayer(round->hochzeitPlayer());
    ui->comboBoxHochzeitFellow->addPlayers(round->playingPlayers());
    ui->comboBoxHochzeitFellow->setCurrentPlayer(round->re2Player());
    ui->comboBoxHochzeitSchweine->addPlayers(round->playingPlayers());
    ui->comboBoxHochzeitSchweine->setCurrentPlayer(round->schweinereiPlayer());

    ui->comboBoxSoloPlayer->addPlayers(round->playingPlayers());
    ui->comboBoxSoloPlayer->setCurrentPlayer(round->soloPlayer());
    ui->comboBoxSoloSchweine->addPlayers(round->playingPlayers());
    ui->comboBoxSoloSchweine->setCurrentPlayer(round->schweinereiPlayer());
    ui->comboBoxSoloHochzeit->addPlayers(round->playingPlayers());
    ui->comboBoxSoloHochzeit->setCurrentPlayer(round->schweinereiPlayer());
    ui->checkBoxSoloPflicht->setEnabled(game->mitPflichtSolo());
    ui->comboBoxSoloType->addItems(Round::soloTypeStrings());
    if(round->isSolo())
        ui->comboBoxSoloType->setCurrentText(round->soloTypeString());
    else
        ui->comboBoxSoloType->setCurrentIndex(0);

    ui->comboBoxTrumpfabgabePlayer->addPlayers(round->playingPlayers());
    ui->comboBoxTrumpfabgabePlayer->setCurrentPlayer(round->trumpfabgabePlayer());
    ui->comboBoxTrumpfabgabeAccept->addPlayers(round->playingPlayers());
    ui->comboBoxTrumpfabgabeAccept->setCurrentPlayer(round->re2Player());
    ui->comboBoxTrumpfabgabeSchweine->addPlayers(round->playingPlayers());
    ui->comboBoxTrumpfabgabeSchweine->setCurrentPlayer(round->schweinereiPlayer());

    ui->comboBoxWinner->addItem(tr("Re"));
    ui->comboBoxWinner->addItem(tr("Contra"));

    ui->comboBoxTrumpfabgabeWinner->addItem(tr("Re"));
    ui->comboBoxTrumpfabgabeWinner->addItem(tr("Contra"));

    ui->comboBoxSoloWinner->addItem(tr("Re"));
    ui->comboBoxSoloWinner->addItem(tr("Contra"));

    ui->comboBoxHochzeitWinner->addItem(tr("Re"));
    ui->comboBoxHochzeitWinner->addItem(tr("Contra"));
}

void NewRoundDialog::setCurrentPage(int index)
{
    ui->stackedWidget->setCurrentIndex(index);

    switch(index) {
    case 0:
        checkNormalRoundContents();
        break;
    case 1:
        checkHochzeitRoundContents();
        break;
    case 2:
        checkSoloRoundContents();
        break;
    case 3:
        checkTrumpfabgabeRoundContents();
        break;
    default:
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    }
}

void NewRoundDialog::checkNormalRoundContents()
{
    ui->comboBoxNormalRe1->addPlayers(m_doppelkopfRound->playingPlayers());
    ui->comboBoxNormalRe2->addPlayers(m_doppelkopfRound->playingPlayers());

    if(ui->comboBoxNormalRe1->currentPlayer()) {
        ui->comboBoxNormalRe2->removePlayer(ui->comboBoxNormalRe1->currentPlayer());
    }
    if(ui->comboBoxNormalRe2->currentPlayer()) {
        ui->comboBoxNormalRe1->removePlayer(ui->comboBoxNormalRe2->currentPlayer());
    }

    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(ui->comboBoxNormalRe1->currentPlayer() &&
                                                              ui->comboBoxNormalRe2->currentPlayer());

    if(ui->spinBoxNormalPoints->value() == 0)
    {
        ui->comboBoxWinner->setEnabled(true);
    }
    else
    {
        ui->comboBoxWinner->setEnabled(false);

        if(ui->spinBoxNormalPoints->value() > 0)
        {
            ui->comboBoxWinner->setCurrentIndex(ui->comboBoxWinner->findText("Re"));
        }
        if(ui->spinBoxNormalPoints->value() < 0)
        {
            ui->comboBoxWinner->setCurrentIndex(ui->comboBoxWinner->findText("Contra"));
        }
    }
}

void NewRoundDialog::checkHochzeitRoundContents()
{
    ui->comboBoxHochzeitHochzeit->addPlayers(m_doppelkopfRound->playingPlayers());
    ui->comboBoxHochzeitFellow->addPlayers(m_doppelkopfRound->playingPlayers());

    if(ui->comboBoxHochzeitHochzeit->currentPlayer()) {
        ui->comboBoxHochzeitFellow->removePlayer(ui->comboBoxHochzeitHochzeit->currentPlayer());
    }
    if(ui->comboBoxHochzeitFellow->currentPlayer()) {
        ui->comboBoxHochzeitHochzeit->removePlayer(ui->comboBoxHochzeitFellow->currentPlayer());
    }

    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(ui->comboBoxHochzeitHochzeit->currentPlayer() &&
                                                              ui->comboBoxHochzeitFellow->currentPlayer());

    if(ui->spinBoxHochzeitPoints->value() == 0)
    {
        ui->comboBoxHochzeitWinner->setEnabled(true);
    }
    else
    {
        ui->comboBoxHochzeitWinner->setEnabled(false);

        if(ui->spinBoxHochzeitPoints->value() > 0)
        {
            ui->comboBoxHochzeitWinner->setCurrentIndex(ui->comboBoxHochzeitWinner->findText("Re"));
        }
        if(ui->spinBoxHochzeitPoints->value() < 0)
        {
            ui->comboBoxHochzeitWinner->setCurrentIndex(ui->comboBoxHochzeitWinner->findText("Contra"));
        }
    }
}

void NewRoundDialog::checkSoloRoundContents()
{
    QSharedPointer<Game> game = static_cast<QSharedPointer<Game> >(m_doppelkopfRound->game());
    if(ui->comboBoxSoloPlayer->currentPlayer() && game->hasPflichtSolo(ui->comboBoxSoloPlayer->currentPlayer())) {
        ui->checkBoxSoloPflicht->setChecked(false);
        ui->checkBoxSoloPflicht->setEnabled(false);
    }
    else {
        ui->checkBoxSoloPflicht->setChecked(true);
        ui->checkBoxSoloPflicht->setEnabled(true);
    }

    if(ui->comboBoxSoloType->currentText() == "Trumpf") {
        ui->comboBoxSoloSchweine->setEnabled(true);
        ui->comboBoxSoloHochzeit->setEnabled(true);
    }
    else {
        ui->comboBoxSoloSchweine->setEnabled(false);
        ui->comboBoxSoloHochzeit->setEnabled(false);
        ui->comboBoxSoloSchweine->setCurrentIndex(0);
        ui->comboBoxSoloHochzeit->setCurrentIndex(0);
    }

    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(ui->comboBoxSoloPlayer->currentPlayer());

    if(ui->spinBoxSoloPoints->value() == 0)
    {
        ui->comboBoxSoloWinner->setEnabled(true);
    }
    else
    {
        ui->comboBoxSoloWinner->setEnabled(false);

        if(ui->spinBoxSoloPoints->value() > 0)
        {
            ui->comboBoxSoloWinner->setCurrentIndex(ui->comboBoxSoloWinner->findText("Re"));
        }
        if(ui->spinBoxSoloPoints->value() < 0)
        {
            ui->comboBoxSoloWinner->setCurrentIndex(ui->comboBoxSoloWinner->findText("Contra"));
        }
    }
}

void NewRoundDialog::checkTrumpfabgabeRoundContents()
{
    ui->comboBoxTrumpfabgabePlayer->addPlayers(m_doppelkopfRound->playingPlayers());
    ui->comboBoxTrumpfabgabeAccept->addPlayers(m_doppelkopfRound->playingPlayers());

    if(ui->comboBoxTrumpfabgabePlayer->currentPlayer()) {
        ui->comboBoxTrumpfabgabeAccept->removePlayer(ui->comboBoxTrumpfabgabePlayer->currentPlayer());
    }
    if(ui->comboBoxTrumpfabgabeAccept->currentPlayer()) {
        ui->comboBoxTrumpfabgabePlayer->removePlayer(ui->comboBoxTrumpfabgabeAccept->currentPlayer());
    }

    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(ui->comboBoxTrumpfabgabePlayer->currentPlayer() &&
                                                              ui->comboBoxTrumpfabgabeAccept->currentPlayer());

    if(ui->spinBoxTrumpfabgabePoints->value() == 0)
    {
        ui->comboBoxTrumpfabgabeWinner->setEnabled(true);
    }
    else
    {
        ui->comboBoxTrumpfabgabeWinner->setEnabled(false);

        if(ui->spinBoxTrumpfabgabePoints->value() > 0)
        {
            ui->comboBoxTrumpfabgabeWinner->setCurrentIndex(ui->comboBoxTrumpfabgabeWinner->findText("Re"));
        }
        if(ui->spinBoxTrumpfabgabePoints->value() < 0)
        {
            ui->comboBoxTrumpfabgabeWinner->setCurrentIndex(ui->comboBoxTrumpfabgabeWinner->findText("Contra"));
        }
    }
}

void NewRoundDialog::save()
{
    switch(ui->buttonGroup->checkedId()) {
    case 0:
        saveNormalRound();
        break;
    case 1:
        saveHochzeitRound();
        break;
    case 2:
        saveSoloRound();
        break;
    case 3:
        saveTrumpfabgabeRound();
        break;
    default:
        break;
    }
    accept();
}

void NewRoundDialog::saveNormalRound()
{
    m_doppelkopfRound->setRe1Player(ui->comboBoxNormalRe1->currentPlayer());
    m_doppelkopfRound->setRe2Player(ui->comboBoxNormalRe2->currentPlayer());
    m_doppelkopfRound->setComment(ui->textEditNormalComment->toPlainText());
    if(ui->comboBoxNormalSchweine->currentPlayer())
        m_doppelkopfRound->setSchweinereiPlayer(ui->comboBoxNormalSchweine->currentPlayer());

    foreach(QSharedPointer<Player> p, m_doppelkopfRound->playingPlayers()) {
        if(p == ui->comboBoxNormalRe1->currentPlayer() ||
                p == ui->comboBoxNormalRe2->currentPlayer()) {
            m_doppelkopfRound->setPoints(p, ui->spinBoxNormalPoints->value());
        }
        else {
            m_doppelkopfRound->setPoints(p, -ui->spinBoxNormalPoints->value());
        }
    }

    if(ui->comboBoxWinner->currentText() == "Re")
    {
        m_doppelkopfRound->setWinnerParty(Round::Re);
    }
    else
    {
        m_doppelkopfRound->setWinnerParty(Round::Contra);
    }

    QSharedPointer<Game> game = m_doppelkopfRound->game();
    game->startNextRound();
}

void NewRoundDialog::saveHochzeitRound()
{
    m_doppelkopfRound->setHochzeitPlayer(ui->comboBoxHochzeitHochzeit->currentPlayer());
    m_doppelkopfRound->setRe1Player(ui->comboBoxHochzeitHochzeit->currentPlayer());
    m_doppelkopfRound->setRe2Player(ui->comboBoxHochzeitFellow->currentPlayer());
    m_doppelkopfRound->setComment(ui->textEditHochzeitComment->toPlainText());
    if(ui->comboBoxHochzeitSchweine->currentPlayer())
        m_doppelkopfRound->setSchweinereiPlayer(ui->comboBoxHochzeitSchweine->currentPlayer());

    foreach(QSharedPointer<Player> p, m_doppelkopfRound->playingPlayers()) {
        if(p == ui->comboBoxHochzeitHochzeit->currentPlayer() ||
                p == ui->comboBoxHochzeitFellow->currentPlayer()) {
            m_doppelkopfRound->setPoints(p, ui->spinBoxHochzeitPoints->value());
        }
        else {
            m_doppelkopfRound->setPoints(p, -ui->spinBoxHochzeitPoints->value());
        }
    }

    if(ui->comboBoxWinner->currentText() == "Re")
    {
        m_doppelkopfRound->setWinnerParty(Round::Re);
    }
    else
    {
        m_doppelkopfRound->setWinnerParty(Round::Contra);
    }

    QSharedPointer<Game> game = m_doppelkopfRound->game();
    game->startNextRound();
}

void NewRoundDialog::saveSoloRound()
{
    m_doppelkopfRound->setSoloPlayer(ui->comboBoxSoloPlayer->currentPlayer());
    m_doppelkopfRound->setRe1Player(ui->comboBoxSoloPlayer->currentPlayer());
    m_doppelkopfRound->setComment(ui->textEditSoloComment->toPlainText());
    if(ui->comboBoxSoloSchweine->currentPlayer())
        m_doppelkopfRound->setSchweinereiPlayer(ui->comboBoxSoloSchweine->currentPlayer());
    if(ui->comboBoxSoloHochzeit->currentPlayer())
        m_doppelkopfRound->setHochzeitPlayer(ui->comboBoxSoloHochzeit->currentPlayer());
    m_doppelkopfRound->setSoloType(Round::soloTypeFromString(ui->comboBoxSoloType->currentText()));
    m_doppelkopfRound->setIsPflicht(ui->checkBoxSoloPflicht->isChecked());

    foreach(QSharedPointer<Player> p, m_doppelkopfRound->playingPlayers()) {
        if(p == ui->comboBoxSoloPlayer->currentPlayer()) {
            m_doppelkopfRound->setPoints(p, 3 * ui->spinBoxSoloPoints->value());
        }
        else {
            m_doppelkopfRound->setPoints(p, -ui->spinBoxSoloPoints->value());
        }
    }

    if(ui->comboBoxWinner->currentText() == "Re")
    {
        m_doppelkopfRound->setWinnerParty(Round::Re);
    }
    else
    {
        m_doppelkopfRound->setWinnerParty(Round::Contra);
    }

    QSharedPointer<Game> game = m_doppelkopfRound->game();
    game->startNextRound();
}

void NewRoundDialog::saveTrumpfabgabeRound()
{
    m_doppelkopfRound->setTrumpfabgabePlayer(ui->comboBoxTrumpfabgabePlayer->currentPlayer());
    m_doppelkopfRound->setRe1Player(ui->comboBoxTrumpfabgabePlayer->currentPlayer());
    m_doppelkopfRound->setRe2Player(ui->comboBoxTrumpfabgabeAccept->currentPlayer());
    m_doppelkopfRound->setComment(ui->textEditTrumpfabgabeComment->toPlainText());
    if(ui->comboBoxTrumpfabgabeSchweine->currentPlayer())
        m_doppelkopfRound->setSchweinereiPlayer(ui->comboBoxTrumpfabgabeSchweine->currentPlayer());

    foreach(QSharedPointer<Player> p, m_doppelkopfRound->playingPlayers()) {
        if(p == ui->comboBoxTrumpfabgabePlayer->currentPlayer() ||
                p == ui->comboBoxTrumpfabgabeAccept->currentPlayer()) {
            m_doppelkopfRound->setPoints(p, ui->spinBoxTrumpfabgabePoints->value());
        }
        else {
            m_doppelkopfRound->setPoints(p, -ui->spinBoxTrumpfabgabePoints->value());
        }
    }

    if(ui->comboBoxWinner->currentText() == "Re")
    {
        m_doppelkopfRound->setWinnerParty(Round::Re);
    }
    else
    {
        m_doppelkopfRound->setWinnerParty(Round::Contra);
    }

    QSharedPointer<Game> game = m_doppelkopfRound->game();
    game->startNextRound();
}

void NewRoundDialog::on_buttonBox_rejected()
{
    reject();
}

QSharedPointer<Round> NewRoundDialog::doppelkopfRound() const
{
    return m_doppelkopfRound;
}
