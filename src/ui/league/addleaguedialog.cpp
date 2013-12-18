#include "addleaguedialog.h"
#include "ui_addleaguedialog.h"

#include <QPushButton>

#include <data/league.h>

#include <ui/widgets/playerslistwidget.h>

AddLeagueDialog::AddLeagueDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLeagueDialog)
{
    ui->setupUi(this);

    connect(ui->lineEditName, SIGNAL(editingFinished()), this, SLOT(checkContent()));
    connect(ui->doubleSpinBoxRatio, SIGNAL(valueChanged(double)), this, SLOT(checkContent()));
    connect(ui->dateEditFrom, SIGNAL(dateChanged(QDate)), this, SLOT(checkContent()));
    connect(ui->dateEditTo, SIGNAL(dateChanged(QDate)), this, SLOT(checkContent()));

    m_possiblePlayers = new PlayersListWidget(this);
    m_possiblePlayers->add(Qp::readAll<Player>());
    ui->verticalLayoutPossiblePlayers->addWidget(m_possiblePlayers);
    connect(m_possiblePlayers, SIGNAL(playerActivated(QSharedPointer<Player>)), this, SLOT(onPossiblePlayersItemActivated(QSharedPointer<Player>)));

    m_selectedPlayers = new PlayersListWidget(this);
    ui->verticalLayoutSelectedPlayers->addWidget(m_selectedPlayers);
    connect(m_selectedPlayers, SIGNAL(playerActivated(QSharedPointer<Player>)), this, SLOT(onSelectedPlayersItemActivated(QSharedPointer<Player>)));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

AddLeagueDialog::~AddLeagueDialog()
{
    delete ui;
}

void AddLeagueDialog::checkContent()
{
    bool check = true;

    if(ui->lineEditName->text() == "") {
        check = false;
    }

    if(ui->dateEditFrom->date() >= ui->dateEditTo->date()) {
        check = false;
    }

    if(m_selectedPlayers->items().size() < 3) {
        check = false;
    }

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(check);
}

void AddLeagueDialog::onPossiblePlayersItemActivated(QSharedPointer<Player> player)
{
    m_selectedPlayers->add(player);
    m_possiblePlayers->remove(player);

    checkContent();
}

void AddLeagueDialog::onSelectedPlayersItemActivated(QSharedPointer<Player> player)
{
    m_possiblePlayers->add(player);
    m_selectedPlayers->remove(player);

    checkContent();
}

void AddLeagueDialog::on_buttonBox_accepted()
{
    QSharedPointer<League> league = Qp::create<League>();
    league->setPlayerRatio(ui->doubleSpinBoxRatio->value());
    league->setStartDate(ui->dateEditFrom->date());
    league->setEndDate(ui->dateEditTo->date());
    league->setName(ui->lineEditName->text());
    league->setPlayers(m_selectedPlayers->items());

    Qp::update(league);
}
