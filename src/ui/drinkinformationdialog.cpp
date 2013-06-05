#include "drinkinformationdialog.h"
#include "ui_drinkinformationdialog.h"

#include <data/drink.h>

DrinkInformationDialog::DrinkInformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrinkInformationDialog)
{
    ui->setupUi(this);

    ui->comboBoxType->addItems(Drink::typeStrings());
}

DrinkInformationDialog::~DrinkInformationDialog()
{
    delete ui;
}

QSharedPointer<Drink> DrinkInformationDialog::drink() const
{
    return m_drink;
}

void DrinkInformationDialog::setDrink(const QSharedPointer<Drink> &drink)
{
    if(!drink)
        return;

    m_drink = drink;
    ui->lineEditName->setText(drink->name());
    ui->lineEditAlcohol->setText(QString::number(drink->alcoholByVolume()));
    ui->lineEditVolume->setText(QString::number(drink->volume()));
    int typeIndex = ui->comboBoxType->findText(drink->typeString());
    if(typeIndex >= 0) {
        ui->comboBoxType->setCurrentIndex(typeIndex);
    }
    ui->picture->setPixmap(m_drink->picture());
    ui->labelCount->setText(QString::number(drink->liveDrinks().size()));
}

void DrinkInformationDialog::accept()
{
    if(!m_drink)
        return;

    m_drink->setName(ui->lineEditName->text());
    bool ok = false;
    double alc = ui->lineEditAlcohol->text().toDouble(&ok);
    if(ok)
        m_drink->setAlcoholByVolume(alc);

    double vol = ui->lineEditVolume->text().toDouble(&ok);
    if(ok)
        m_drink->setVolume(vol);

    Drink::Type type = Drink::typeFromString(ui->comboBoxType->currentText());
    m_drink->setType(type);

    m_drink->setPicture(ui->picture->pixmap());
    Qp::update(m_drink);
    QDialog::accept();
}
