#include "addphotosdialog.h"
#include "ui_addphotosdialog.h"

#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QDate>
#include <QMessageBox>

#include <QImageMetaData.h>
#include <QPersistence.h>
#include <data/game.h>
#include <data/place.h>
#include <data/round.h>
#include <data/player.h>
#include <library.h>
#include <misc/tools.h>

AddPhotosDialog::AddPhotosDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPhotosDialog),
    m_currentFile("")
{
    ui->setupUi(this);

    QList<QSharedPointer<Game> > games = Qp::readAll<Game>();
    qSort(games.begin(), games.end(), sortGamesByDateLastFirst);
    ui->comboBoxGame->setObjects(games);

    QList<QSharedPointer<Player>> players = Qp::readAll<Player>();
    qSort(players.begin(), players.end(), sortPlayersByLastGame);
    ui->comboBoxDefaultPlayer->setObjects(players);

    ui->comboBoxRound->setEnabled(false);
    ui->comboBoxPlayer->setEnabled(false);
}

AddPhotosDialog::~AddPhotosDialog()
{
    delete ui;
}

void AddPhotosDialog::setFilesToAdd(QStringList list)
{
    m_filesToAdd = list;
}

void AddPhotosDialog::on_pushButtonCancel_clicked()
{
    this->reject();
}

void AddPhotosDialog::on_pushButtonNext_clicked()
{
    if(!m_currentFile.isEmpty()) {
        processFile();
    }

    if(m_filesToAdd.size() == 1) {
        ui->pushButtonSkip->setText("Überspringen && Fertig");
        ui->pushButtonNext->setText("Fertig");
    }
    else if(m_filesToAdd.isEmpty()) {
        if(m_processedFiles.isEmpty()) {
            this->reject();
            return;
        }
        this->accept();
        return;
    }

    loadFile(m_filesToAdd.takeFirst());
}

void AddPhotosDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    if(m_filesToAdd.isEmpty()) {
        this->reject();
    }

    on_pushButtonNext_clicked();
}

void AddPhotosDialog::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.exists()) {
        emit processFinishedWithError("File not found: " + fileName);
    }

    QImage image(fileName);
    if(image.isNull()) {
        emit processFinishedWithError("Cannot load file: " + fileName);
    }

    m_currentFile = fileName;

    ui->labelImage->setPixmap(QPixmap::fromImage(image).scaledToHeight(ui->labelImage->height()));

    QImageMetaData* metadata = new QImageMetaData();
    QDateTime creationDate;
    if (metadata->read(fileName)) {
        creationDate = QDateTime::fromString(metadata->dateAndTimeOriginal(), "yyyy:MM:dd hh:mm:ss");
        ui->labelDate->setText(creationDate.toString("dd.MM.yyyy hh:mm"));
        ui->labelCamera->setText(metadata->make() + " " + metadata->model());
    }

    delete(metadata);
}

void AddPhotosDialog::processFile()
{
    if(m_currentFile.isEmpty()) {
        qWarning() << "AddPhotosDialog:: No current file to process";
        return;
    }

    QString packagePath = Library::instance()->packagePath();

    QString path = packagePath + "/photos";
    if(ui->comboBoxGame->currentObject()) {
        path += "/" + QString::number(Qp::primaryKey(ui->comboBoxGame->currentObject()));
        if(ui->comboBoxRound->currentObject()) {
            path += "/" + QString::number(ui->comboBoxRound->currentObject()->number());
        }
    }

    if(!QDir().mkpath(path)) {
        qWarning() << "AddPhotosDialog::Could not create folder structure!";
        qDebug() << "Path: " << path;
        return;
    }

    QString newFile;
    if(ui->comboBoxPlayer->currentObject()) {
        newFile = QString::number(Qp::primaryKey(ui->comboBoxPlayer->currentObject())) + "." + QFileInfo(m_currentFile).suffix();
        int count = 0;
        while(QFile(path + "/" + newFile).exists()) {
            newFile = QString::number(Qp::primaryKey(ui->comboBoxPlayer->currentObject())) + "_" + QString::number(count) + "." + QFileInfo(m_currentFile).suffix();
            count++;
        }
    }
    else{
        newFile = "Photo." + QFileInfo(m_currentFile).suffix();
        int count = 0;
        while(QFile(path + "/" + newFile).exists()) {
            newFile = "Photo_" + QString::number(count) + "." + QFileInfo(m_currentFile).suffix();
            count++;
        }
    }

    QString newFilePath = path + "/" + newFile;

    qDebug() << "File to process: " << m_currentFile;
    qDebug() << "New filePath: " << newFilePath;
    if(!QFile::copy(m_currentFile, newFilePath)) {
        qWarning() << "Copying file failed!";
    }

    m_processedFiles.append(newFilePath);
}

int AddPhotosDialog::distance(QDateTime one, QDateTime two)
{
    int distance = one.secsTo(two);
    if(distance < 0) {
        distance = -distance;
    }

    return distance;
}

void AddPhotosDialog::on_comboBoxGame_currentIndexChanged(int)
{
    if(!ui->comboBoxGame->currentObject()) {
        ui->labelPlace->setText("");
        ui->labelGameDate->setText("");
        ui->comboBoxRound->clear();
        ui->comboBoxRound->setEnabled(false);
        return;
    }

    ui->labelPlace->setText(ui->comboBoxGame->currentObject()->site()->displayString());
    ui->labelGameDate->setText(ui->comboBoxGame->currentObject()->creationTime().toString("dd.MM.yyyy hh:mm"));

    ui->comboBoxRound->setEnabled(true);
    ui->comboBoxRound->setObjects(ui->comboBoxGame->currentObject()->rounds());
}

void AddPhotosDialog::on_comboBoxRound_currentIndexChanged(int)
{    
    if(!ui->comboBoxRound->currentObject()) {
        ui->comboBoxPlayer->setEnabled(false);
        return;
    }

    ui->labelRePlayers->setText(Tools::playersString(ui->comboBoxRound->currentObject()->rePlayers()).toString());
    ui->labelContraPlayers->setText(Tools::playersString(ui->comboBoxRound->currentObject()->contraPlayers()).toString());
    ui->labelType->setText(ui->comboBoxRound->currentObject()->typeString());

    ui->comboBoxPlayer->setEnabled(true);
    ui->comboBoxPlayer->setObjects(ui->comboBoxRound->currentObject()->playingPlayers());
}

void AddPhotosDialog::on_pushButtonIdentify_clicked()
{
    QDateTime time = QDateTime::fromString(ui->labelDate->text(), "dd.MM.yyyy hh:mm");

    QSharedPointer<Round> identifiedRound;

    int mindistance = std::numeric_limits<int>::max();
    foreach(QSharedPointer<Round> round, Qp::readAll<Round>()) {
        int temp = distance(time, round->startTime());
        if(temp < mindistance) {
            mindistance = temp;
            identifiedRound = round;
        }
    }


    if(!identifiedRound || !identifiedRound->game())
        return;

    ui->comboBoxGame->setCurrentObject(identifiedRound->game());
    ui->comboBoxRound->setCurrentObject(identifiedRound);

    if(!ui->comboBoxDefaultPlayer->currentObject())
        return;

    if(identifiedRound->playingPlayers().contains(ui->comboBoxDefaultPlayer->currentObject())) {
        ui->comboBoxPlayer->setCurrentObject(ui->comboBoxDefaultPlayer->currentObject());
    }
    else{
        ui->comboBoxPlayer->setCurrentIndex(0);
    }
}


void AddPhotosDialog::on_pushButtonSkip_clicked()
{
    if(m_filesToAdd.size() == 1) {
        ui->pushButtonSkip->setText("Überspringen && Fertig");
        ui->pushButtonNext->setText("Fertig");
    }
    else if(m_filesToAdd.isEmpty()) {
        if(m_processedFiles.isEmpty()) {
            this->reject();
            return;
        }
        this->accept();
        return;
    }

    loadFile(m_filesToAdd.takeFirst());
}

QStringList AddPhotosDialog::processedFiles()
{
    return m_processedFiles;
}

void AddPhotosDialog::on_comboBoxDefaultPlayer_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    if(!ui->comboBoxDefaultPlayer->currentObject())
        return;

    ui->comboBoxPlayer->setCurrentObject(ui->comboBoxDefaultPlayer->currentObject());
}
