#include "newrounddialog.h"
#include "ui_newrounddialog.h"

#include <data/round.h>
#include <data/game.h>
#include <data/player.h>
#include <misc/tools.h>

#include <QFile>
#include <QPushButton>

NewRoundDialog::NewRoundDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewRoundDialog),
    m_doppelkopfRound(0)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

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

    connect(ui->spinBoxTrumpfCount, SIGNAL(valueChanged(int)),
            this, SLOT(checkTrumpfabgabeRoundContents()));

    connect(ui->buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked()), this, SLOT(save()));
}

NewRoundDialog::~NewRoundDialog()
{
    delete ui;
}

void NewRoundDialog::setRound(QSharedPointer<Round> round, Context context)
{
    if(!round || m_doppelkopfRound == round)
        return;

    QSharedPointer<Game> game = round->game();
    m_context = context;

    ui->textEditHochzeitComment->setText(round->comment());
    ui->textEditNormalComment->setText(round->comment());
    ui->textEditSoloComment->setText(round->comment());
    ui->textEditTrumpfabgabeComment->setText(round->comment());

    ui->comboBoxWinner->addItem(tr("Re"));
    ui->comboBoxWinner->addItem(tr("Contra"));

    ui->comboBoxTrumpfabgabeWinner->addItem(tr("Re"));
    ui->comboBoxTrumpfabgabeWinner->addItem(tr("Contra"));

    ui->comboBoxSoloWinner->addItem(tr("Re"));
    ui->comboBoxSoloWinner->addItem(tr("Contra"));

    ui->comboBoxHochzeitWinner->addItem(tr("Re"));
    ui->comboBoxHochzeitWinner->addItem(tr("Contra"));

    ui->spinBoxHochzeitContraPoints->setValue(round->contraGamePoints());
    ui->spinBoxNormalContraPoints->setValue(round->contraGamePoints());
    ui->spinBoxSoloContraPoints->setValue(round->contraGamePoints());
    ui->spinBoxTrumpfabgabeContraPoints->setValue(round->contraGamePoints());

    ui->spinBoxHochzeitRePoints->setValue(round->reGamePoints());
    ui->spinBoxNormalRePoints->setValue(round->reGamePoints());
    ui->spinBoxSoloRePoints->setValue(round->reGamePoints());
    ui->spinBoxTrumpfabgabeRePoints->setValue(round->reGamePoints());

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

    if(m_doppelkopfRound->hochzeitDecision() == Round::UnkownHochzeitDecision) {
        if(m_doppelkopfRound->state() == Round::Finished) {
            ui->comboBoxHochzeitDecision->insertItem(0, tr("???"));
        }
        ui->comboBoxHochzeitDecision->setCurrentIndex(0);
    }
    else {
        ui->comboBoxHochzeitDecision->setCurrentText(textFromHochzeitDecision(m_doppelkopfRound->hochzeitDecision()));
    }

    ui->comboBoxSoloPlayer->addPlayers(round->playingPlayers());
    ui->comboBoxSoloPlayer->setCurrentPlayer(round->soloPlayer());
    ui->comboBoxSoloSchweine->addPlayers(round->playingPlayers());
    ui->comboBoxSoloSchweine->setCurrentPlayer(round->schweinereiPlayer());
    ui->comboBoxSoloHochzeit->addPlayers(round->playingPlayers());
    ui->comboBoxSoloHochzeit->setCurrentPlayer(round->schweinereiPlayer());
    ui->checkBoxSoloPflicht->setEnabled(game->mitPflichtSolo());
    ui->comboBoxSoloType->addItems(Round::soloTypeStrings());
    ui->comboBoxSoloHochzeitDecision->setCurrentText(textFromHochzeitDecision(m_doppelkopfRound->hochzeitDecision()));

    // TODO: Wird nicht bei neuer Runde ausgeführt
    if(round->isSolo()) {
        ui->comboBoxSoloType->setCurrentText(round->soloTypeString());
        if(round->soloTypeString() == "Trumpf" || round->soloTypeString() == "Sitzengelassene Hochzeit"
                || round->soloTypeString() == "Stille Hochzeit" || round->soloTypeString() == "Falsch gespielt") {
            if(round->hochzeitPlayer()) {
                ui->comboBoxSoloHochzeit->setCurrentPlayer(round->hochzeitPlayer());
            }
            if(round->schweinereiPlayer()) {
                ui->comboBoxSoloSchweine->setCurrentPlayer(round->schweinereiPlayer());
            }
        }
    }
    else {
        ui->comboBoxSoloType->setCurrentIndex(0);
    }

    ui->comboBoxTrumpfabgabePlayer->addPlayers(round->playingPlayers());
    ui->comboBoxTrumpfabgabePlayer->setCurrentPlayer(round->trumpfabgabePlayer());
    ui->comboBoxTrumpfabgabeAccept->addPlayers(round->playingPlayers());
    ui->comboBoxTrumpfabgabeAccept->setCurrentPlayer(round->re2Player());
    ui->comboBoxTrumpfabgabeSchweine->addPlayers(round->playingPlayers());
    ui->comboBoxTrumpfabgabeSchweine->setCurrentPlayer(round->schweinereiPlayer());

    if(round->state() == Round::Finished) {
        ui->spinBoxNormalPoints->setValue(round->points(round->re1Player()));
        ui->spinBoxTrumpfabgabePoints->setValue(round->points(round->re1Player()));
        ui->spinBoxHochzeitPoints->setValue(round->points(round->re1Player()));

        if(round->trumpfCount() < 0) {
            ui->spinBoxTrumpfCount->setMinimum(0);
            ui->spinBoxTrumpfCount->setSpecialValueText(tr("?"));
            ui->spinBoxTrumpfCount->setValue(0); // Displays as '?'
            ui->spinBoxTrumpfZurueck->setEnabled(false);
        }
        else {
            ui->spinBoxTrumpfZurueck->setMaximum(round->trumpfCount());
            ui->spinBoxTrumpfZurueck->setValue(round->trumpfZurueck());
            ui->spinBoxTrumpfCount->setValue(round->trumpfCount());
        }

        if(round->isSolo()) {
            ui->spinBoxSoloPoints->setValue(round->points(round->re1Player()) / 3);
        }
        else {
            ui->spinBoxSoloPoints->setValue(round->points(round->re1Player()));
        }
    }

    setCurrentPage(round->type());
}

void NewRoundDialog::setCurrentPage(Round::Type type)
{
    ui->stackedWidget->setCurrentIndex(type);

    switch(type) {
    case Round::NormalRound:
        checkNormalRoundContents();
        break;
    case Round::Hochzeit:
        checkHochzeitRoundContents();
        break;
    case Round::Solo:
        checkSoloRoundContents();
        break;
    case Round::Trumpfabgabe:
        checkTrumpfabgabeRoundContents();
        break;
    default:
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    }
}

void NewRoundDialog::checkNormalRoundContents()
{
    disconnect(ui->comboBoxNormalRe1, SIGNAL(currentIndexChanged(int)), this, SLOT(checkNormalRoundContents()));
    disconnect(ui->comboBoxNormalRe2, SIGNAL(currentIndexChanged(int)), this, SLOT(checkNormalRoundContents()));

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

    connect(ui->comboBoxNormalRe1, SIGNAL(currentIndexChanged(int)), this, SLOT(checkNormalRoundContents()));
    connect(ui->comboBoxNormalRe2, SIGNAL(currentIndexChanged(int)), this, SLOT(checkNormalRoundContents()));
}

void NewRoundDialog::checkHochzeitRoundContents()
{
    disconnect(ui->comboBoxHochzeitHochzeit, SIGNAL(currentIndexChanged(int)), this, SLOT(checkHochzeitRoundContents()));
    disconnect(ui->comboBoxHochzeitFellow, SIGNAL(currentIndexChanged(int)), this, SLOT(checkHochzeitRoundContents()));

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

    connect(ui->comboBoxHochzeitHochzeit, SIGNAL(currentIndexChanged(int)), this, SLOT(checkHochzeitRoundContents()));
    connect(ui->comboBoxHochzeitFellow, SIGNAL(currentIndexChanged(int)), this, SLOT(checkHochzeitRoundContents()));
}

void NewRoundDialog::checkSoloRoundContents()
{
    QSharedPointer<Game> game = static_cast<QSharedPointer<Game> >(m_doppelkopfRound->game());

    ui->comboBoxSoloHochzeitDecision->setEnabled(false);
    if(ui->comboBoxSoloHochzeitDecision->count() == 2
            && ui->comboBoxSoloType->currentText() != "Sitzengelassene Hochzeit") {
        ui->comboBoxSoloHochzeitDecision->insertItem(0, tr("---"));
        ui->comboBoxSoloHochzeitDecision->setCurrentIndex(0);
    }

    if(ui->comboBoxSoloType->currentText() == "Trumpf"
            || ui->comboBoxSoloType->currentText() == "Falsch gespielt") {
        ui->comboBoxSoloSchweine->setEnabled(true);
        ui->comboBoxSoloHochzeit->setEnabled(true);
    }
    else if(ui->comboBoxSoloType->currentText() == "Stille Hochzeit"
            || ui->comboBoxSoloType->currentText() == "Sitzengelassene Hochzeit") {
        ui->comboBoxSoloHochzeit->setEnabled(false);
        ui->comboBoxSoloHochzeit->setCurrentPlayer(ui->comboBoxSoloPlayer->currentPlayer());
        ui->comboBoxSoloSchweine->setEnabled(true);
    }
    else {
        ui->comboBoxSoloSchweine->setEnabled(false);
        ui->comboBoxSoloHochzeit->setEnabled(false);
        ui->comboBoxSoloSchweine->setCurrentIndex(0);
        ui->comboBoxSoloHochzeit->setCurrentIndex(0);
    }

    if(ui->comboBoxSoloType->currentText() == "Sitzengelassene Hochzeit"
            || ui->comboBoxSoloType->currentText() == "Falsch gespielt") {
        if(ui->comboBoxSoloHochzeitDecision->count() == 3)
            ui->comboBoxSoloHochzeitDecision->removeItem(0);

        ui->comboBoxSoloHochzeitDecision->setEnabled(true);
    }

    bool isPflicht = m_doppelkopfRound->isPflicht();
    bool hasPflicht = game->hasPflichtSolo(ui->comboBoxSoloPlayer->currentPlayer());
    if(m_doppelkopfRound->state() == Round::Finished) {
        // Allow to check the box, when it was the pflichtsolo, or when there is no pflichtsolo yet.
        // This allows the user to have only one pflichtsolo per game checked at the same time,
        // but also to correct an errornous entry
        bool checked = isPflicht;
        bool enabled = isPflicht || !hasPflicht;
        ui->checkBoxSoloPflicht->setChecked(checked);
        ui->checkBoxSoloPflicht->setEnabled(enabled);
    }
    else {
        // If the player already has played his pflichtsolo, do not allow to add another pflichtsolo.
        // If the player has not played his pflichtsolo, make the solo pflicht by default.
        bool checked = !hasPflicht && (ui->checkBoxSoloPflicht->isChecked()
                                       || !ui->checkBoxSoloPflicht->isEnabled());
        bool enabled = !hasPflicht;
        ui->checkBoxSoloPflicht->setChecked(checked);
        ui->checkBoxSoloPflicht->setEnabled(enabled);
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
    disconnect(ui->comboBoxTrumpfabgabePlayer, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTrumpfabgabeRoundContents()));
    disconnect(ui->comboBoxTrumpfabgabeAccept, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTrumpfabgabeRoundContents()));

    ui->comboBoxTrumpfabgabePlayer->addPlayers(m_doppelkopfRound->playingPlayers());
    ui->comboBoxTrumpfabgabeAccept->addPlayers(m_doppelkopfRound->playingPlayers());

    ui->spinBoxTrumpfZurueck->setMaximum(ui->spinBoxTrumpfCount->value());

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

    connect(ui->comboBoxTrumpfabgabePlayer, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTrumpfabgabeRoundContents()));
    connect(ui->comboBoxTrumpfabgabeAccept, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTrumpfabgabeRoundContents()));
}

void NewRoundDialog::save()
{
    switch(ui->stackedWidget->currentIndex()) {
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

    Qp::update(m_doppelkopfRound);
    QSharedPointer<Game> game = m_doppelkopfRound->game();
    game->save();

    if(m_context == NewRound) {
        game->startNextRound();
    }

    close();
}

void NewRoundDialog::saveNormalRound()
{
    m_doppelkopfRound->setRe1Player(ui->comboBoxNormalRe1->currentPlayer());
    m_doppelkopfRound->setRe2Player(ui->comboBoxNormalRe2->currentPlayer());
    m_doppelkopfRound->setComment(ui->textEditNormalComment->toPlainText());
    if(ui->comboBoxNormalSchweine->currentPlayer())
        m_doppelkopfRound->setSchweinereiPlayer(ui->comboBoxNormalSchweine->currentPlayer());

    int contraPlayers = 0;
    foreach(QSharedPointer<Player> p, m_doppelkopfRound->playingPlayers()) {
        if(p == ui->comboBoxNormalRe1->currentPlayer() ||
                p == ui->comboBoxNormalRe2->currentPlayer()) {
            m_doppelkopfRound->setPoints(p, ui->spinBoxNormalPoints->value());
        }
        else {
            m_doppelkopfRound->setPoints(p, -ui->spinBoxNormalPoints->value());
            switch(contraPlayers){
            case 0:
                m_doppelkopfRound->setContra1Player(p);
                break;
            case 1:
                m_doppelkopfRound->setContra2Player(p);
                break;
            }
            contraPlayers++;
        }
    }

    m_doppelkopfRound->setReGamePoints(ui->spinBoxNormalRePoints->value());
    m_doppelkopfRound->setContraGamePoints(ui->spinBoxNormalContraPoints->value());

    if(ui->comboBoxWinner->currentText() == "Re")
    {
        m_doppelkopfRound->setWinnerParty(Round::Re);
    }
    else
    {
        m_doppelkopfRound->setWinnerParty(Round::Contra);
    }
}

void NewRoundDialog::saveHochzeitRound()
{
    m_doppelkopfRound->setHochzeitPlayer(ui->comboBoxHochzeitHochzeit->currentPlayer());
    m_doppelkopfRound->setRe1Player(ui->comboBoxHochzeitHochzeit->currentPlayer());
    m_doppelkopfRound->setRe2Player(ui->comboBoxHochzeitFellow->currentPlayer());
    m_doppelkopfRound->setComment(ui->textEditHochzeitComment->toPlainText());
    if(ui->comboBoxHochzeitSchweine->currentPlayer())
        m_doppelkopfRound->setSchweinereiPlayer(ui->comboBoxHochzeitSchweine->currentPlayer());

    int contraPlayers = 0;
    foreach(QSharedPointer<Player> p, m_doppelkopfRound->playingPlayers()) {
        if(p == ui->comboBoxHochzeitHochzeit->currentPlayer() ||
                p == ui->comboBoxHochzeitFellow->currentPlayer()) {
            m_doppelkopfRound->setPoints(p, ui->spinBoxHochzeitPoints->value());
        }
        else {
            m_doppelkopfRound->setPoints(p, -ui->spinBoxHochzeitPoints->value());
            switch(contraPlayers){
            case 0:
                m_doppelkopfRound->setContra1Player(p);
                break;
            case 1:
                m_doppelkopfRound->setContra2Player(p);
                break;
            }
            contraPlayers++;
        }
    }

    m_doppelkopfRound->setHochzeitDecision(hochzeitDecisionFromText(ui->comboBoxHochzeitDecision->currentText()));

    m_doppelkopfRound->setReGamePoints(ui->spinBoxHochzeitRePoints->value());
    m_doppelkopfRound->setContraGamePoints(ui->spinBoxHochzeitContraPoints->value());

    if(ui->comboBoxHochzeitWinner->currentText() == "Re")
    {
        m_doppelkopfRound->setWinnerParty(Round::Re);
    }
    else
    {
        m_doppelkopfRound->setWinnerParty(Round::Contra);
    }
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

    m_doppelkopfRound->setHochzeitDecision(hochzeitDecisionFromText(ui->comboBoxSoloHochzeitDecision->currentText()));

    int contraPlayers = 0;
    foreach(QSharedPointer<Player> p, m_doppelkopfRound->playingPlayers()) {
        if(p == ui->comboBoxSoloPlayer->currentPlayer()) {
            m_doppelkopfRound->setPoints(p, 3 * ui->spinBoxSoloPoints->value());
        }
        else {
            m_doppelkopfRound->setPoints(p, -ui->spinBoxSoloPoints->value());
            switch(contraPlayers){
            case 0:
                m_doppelkopfRound->setContra1Player(p);
                break;
            case 1:
                m_doppelkopfRound->setContra2Player(p);
                break;
            case 2:
                m_doppelkopfRound->setContra3Player(p);
                break;
            }
            contraPlayers++;
        }
    }

    m_doppelkopfRound->setReGamePoints(ui->spinBoxSoloRePoints->value());
    m_doppelkopfRound->setContraGamePoints(ui->spinBoxSoloContraPoints->value());

    if(ui->comboBoxSoloWinner->currentText() == "Re")
    {
        m_doppelkopfRound->setWinnerParty(Round::Re);
    }
    else
    {
        m_doppelkopfRound->setWinnerParty(Round::Contra);
    }
}

void NewRoundDialog::saveTrumpfabgabeRound()
{
    Q_ASSERT_X(ui->spinBoxTrumpfCount->value() > 0,
               Q_FUNC_INFO,
               "Trumpfabgabecount may not be zero??");

    m_doppelkopfRound->setTrumpfabgabePlayer(ui->comboBoxTrumpfabgabePlayer->currentPlayer());
    m_doppelkopfRound->setRe1Player(ui->comboBoxTrumpfabgabePlayer->currentPlayer());
    m_doppelkopfRound->setRe2Player(ui->comboBoxTrumpfabgabeAccept->currentPlayer());
    m_doppelkopfRound->setComment(ui->textEditTrumpfabgabeComment->toPlainText());
    if(ui->comboBoxTrumpfabgabeSchweine->currentPlayer())
        m_doppelkopfRound->setSchweinereiPlayer(ui->comboBoxTrumpfabgabeSchweine->currentPlayer());

    m_doppelkopfRound->setTrumpfCount(ui->spinBoxTrumpfCount->value());
    m_doppelkopfRound->setTrumpfZurueck(ui->spinBoxTrumpfZurueck->value());

    int contraPlayers = 0;
    foreach(QSharedPointer<Player> p, m_doppelkopfRound->playingPlayers()) {
        if(p == ui->comboBoxTrumpfabgabePlayer->currentPlayer() ||
                p == ui->comboBoxTrumpfabgabeAccept->currentPlayer()) {
            m_doppelkopfRound->setPoints(p, ui->spinBoxTrumpfabgabePoints->value());
        }
        else {
            m_doppelkopfRound->setPoints(p, -ui->spinBoxTrumpfabgabePoints->value());
            switch(contraPlayers){
            case 0:
                m_doppelkopfRound->setContra1Player(p);
                break;
            case 1:
                m_doppelkopfRound->setContra2Player(p);
                break;
            }
            contraPlayers++;
        }
    }

    m_doppelkopfRound->setReGamePoints(ui->spinBoxTrumpfabgabeRePoints->value());
    m_doppelkopfRound->setContraGamePoints(ui->spinBoxTrumpfabgabeContraPoints->value());

    if(ui->comboBoxTrumpfabgabeWinner->currentText() == "Re")
    {
        m_doppelkopfRound->setWinnerParty(Round::Re);
    }
    else
    {
        m_doppelkopfRound->setWinnerParty(Round::Contra);
    }
}

void NewRoundDialog::on_buttonBox_rejected()
{
    close();
}

Round::HochzeitDecision NewRoundDialog::hochzeitDecisionFromText(const QString &text)
{
    if(text == "Erster Fehl")
        return Round::ErsterFehl;

    if(text == "Erster Trumpf")
        return Round::ErsterTrumpf;

    return Round::UnkownHochzeitDecision;
}

QString NewRoundDialog::textFromHochzeitDecision(Round::HochzeitDecision d)
{
    switch(d) {
    case Round::ErsterFehl:
        return "Erster Fehl";
    case Round::ErsterTrumpf:
        return "Erster Trumpf";
    case Round::UnkownHochzeitDecision:
    default:
        break;
    }

    return "---";
}

QSharedPointer<Round> NewRoundDialog::doppelkopfRound() const
{
    return m_doppelkopfRound;
}

void NewRoundDialog::on_spinBoxNormalRePoints_valueChanged(int /*arg1*/)
{
    disconnect(ui->spinBoxNormalContraPoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxNormalContraPoints_valueChanged(int)));
    ui->spinBoxNormalContraPoints->setValue(240-ui->spinBoxNormalRePoints->value());
    connect(ui->spinBoxNormalContraPoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxNormalContraPoints_valueChanged(int)));
}

void NewRoundDialog::on_spinBoxNormalContraPoints_valueChanged(int /*arg1*/)
{
    disconnect(ui->spinBoxNormalRePoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxNormalRePoints_valueChanged(int)));
    ui->spinBoxNormalRePoints->setValue(240-ui->spinBoxNormalContraPoints->value());
    connect(ui->spinBoxNormalRePoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxNormalRePoints_valueChanged(int)));
}

void NewRoundDialog::on_spinBoxHochzeitRePoints_valueChanged(int /*arg1*/)
{
    disconnect(ui->spinBoxHochzeitContraPoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxHochzeitContraPoints_valueChanged(int)));
    ui->spinBoxHochzeitContraPoints->setValue(240-ui->spinBoxHochzeitRePoints->value());
    connect(ui->spinBoxHochzeitContraPoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxHochzeitContraPoints_valueChanged(int)));
}

void NewRoundDialog::on_spinBoxHochzeitContraPoints_valueChanged(int /*arg1*/)
{
    disconnect(ui->spinBoxHochzeitRePoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxHochzeitRePoints_valueChanged(int)));
    ui->spinBoxHochzeitRePoints->setValue(240-ui->spinBoxHochzeitContraPoints->value());
    connect(ui->spinBoxHochzeitRePoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxHochzeitRePoints_valueChanged(int)));
}

void NewRoundDialog::on_spinBoxSoloRePoints_valueChanged(int /*arg1*/)
{
    disconnect(ui->spinBoxSoloContraPoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxSoloContraPoints_valueChanged(int)));
    ui->spinBoxSoloContraPoints->setValue(240-ui->spinBoxSoloRePoints->value());
    connect(ui->spinBoxSoloContraPoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxSoloContraPoints_valueChanged(int)));
}

void NewRoundDialog::on_spinBoxSoloContraPoints_valueChanged(int /*arg1*/)
{
    disconnect(ui->spinBoxSoloRePoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxSoloRePoints_valueChanged(int)));
    ui->spinBoxSoloRePoints->setValue(240-ui->spinBoxSoloContraPoints->value());
    connect(ui->spinBoxSoloRePoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxSoloRePoints_valueChanged(int)));
}

void NewRoundDialog::on_spinBoxTrumpfabgabeRePoints_valueChanged(int /*arg1*/)
{
    disconnect(ui->spinBoxTrumpfabgabeContraPoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxTrumpfabgabeContraPoints_valueChanged(int)));
    ui->spinBoxTrumpfabgabeContraPoints->setValue(240-ui->spinBoxTrumpfabgabeRePoints->value());
    connect(ui->spinBoxTrumpfabgabeContraPoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxTrumpfabgabeContraPoints_valueChanged(int)));
}

void NewRoundDialog::on_spinBoxTrumpfabgabeContraPoints_valueChanged(int /*arg1*/)
{
    disconnect(ui->spinBoxTrumpfabgabeRePoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxTrumpfabgabeRePoints_valueChanged(int)));
    ui->spinBoxTrumpfabgabeRePoints->setValue(240-ui->spinBoxTrumpfabgabeContraPoints->value());
    connect(ui->spinBoxTrumpfabgabeRePoints, SIGNAL(valueChanged(int)), this, SLOT(on_spinBoxTrumpfabgabeRePoints_valueChanged(int)));
}
