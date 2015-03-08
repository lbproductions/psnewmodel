#include "placeinformationdialog.h"
#include "ui_placeinformationdialog.h"

#include <data/place.h>
#include <QPushButton>

PlaceInformationDialog::PlaceInformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaceInformationDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->lineEditNumber, SIGNAL(editingFinished()), this, SLOT(checkData()));
    connect(ui->lineEditPLZ, SIGNAL(editingFinished()), this, SLOT(checkData()));
    connect(ui->lineEditStreet, SIGNAL(editingFinished()), this, SLOT(checkData()));
    connect(ui->lineEditTown, SIGNAL(editingFinished()), this, SLOT(checkData()));
}

PlaceInformationDialog::~PlaceInformationDialog()
{
    delete ui;
}

void PlaceInformationDialog::setPlace(QSharedPointer<Place> place)
{
    m_place = place;

    ui->lineEditNumber->setText(QString::number(place->houseNumber()));
    ui->lineEditPLZ->setText(QString::number(place->postalCode()));
    ui->lineEditStreet->setText(place->street());
    ui->lineEditTown->setText(place->city());
    ui->imageWellIcon->setPixmap(place->cityEmblem());

    checkData();
}

void PlaceInformationDialog::checkData()
{
    if(ui->lineEditNumber->text() == "" || ui->lineEditPLZ->text() == "" || ui->lineEditStreet->text() == "" || ui->lineEditTown->text() == "") {
        return;
    }

    bool ok = false;
    ui->lineEditPLZ->text().toInt(&ok);
    if(!ok)
        return;

    ok = false;
    ui->lineEditNumber->text().toInt(&ok);
    if(!ok)
        return;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void PlaceInformationDialog::accept()
{
    bool create = false;

    if(!m_place) {
        m_place = Qp::create<Place>();
        create = true;
    }

    m_place->setCity(ui->lineEditTown->text());
    m_place->setPostalCode(ui->lineEditPLZ->text().toInt());
    m_place->setStreet(ui->lineEditStreet->text());
    m_place->setHouseNumber(ui->lineEditNumber->text().toInt());
    m_place->setCityEmblem(ui->imageWellIcon->pixmap());

    Qp::update(m_place);


    if(create)
        emit placeAdded(m_place);

    QDialog::accept();
}
