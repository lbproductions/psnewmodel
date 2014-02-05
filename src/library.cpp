#include "library.h"

#include <data/drink.h>
#include <data/league.h>
#include <data/livedrink.h>
#include <data/old_offlineGameInformation.h>
#include <data/place.h>
#include <data/schmeisserei.h>

#include <QPersistence.h>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QSqlDatabase>
#include <QStandardPaths>
#include <QMessageBox>
#include <QIcon>
#include <QSqlError>
#include <QSettings>

namespace {
static const QString DATABASE_CONNECTION_NAME("persistence");
}

Library::Library()
{
}

bool Library::open()
{
    if(!setupDatabase())
        return false;

    if(!setupPersistence())
        return false;

    if(!fillCaches())
        return false;

    return true;
}

void Library::close()
{
    if(Qp::database().isOpen())
        Qp::database().close();
}

bool Library::setupDatabase()
{
    QString databaseFilePath = getDatabaseFile();

    if(databaseFilePath.isEmpty())
        return false;

    qDebug() << "Using database: ";
    qDebug() << databaseFilePath;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", DATABASE_CONNECTION_NAME);
    db.setDatabaseName(databaseFilePath);

    if(!db.open()) {
        qDebug() << "Could not open database:";
        qDebug() << db.lastError();
        return false;
    }

    saveFileNameInSettings(databaseFilePath);

    return true;
}

QString Library::getDatabaseFile() const
{
    // command line argument
    QString databaseFilePath = fileNameFromArguments();
    if(!databaseFilePath.isEmpty())
        return databaseFilePath;

    // settings (opened at last start)
    databaseFilePath = fileNameFromSettings();
    if(!databaseFilePath.isEmpty())
        return databaseFilePath;

    // dropbox
    databaseFilePath = fileNameInDropbox();
    if(!databaseFilePath.isEmpty())
        return databaseFilePath;

    // local file in QStandardPaths::DataLocation as last
    return fileNameLocal();
}

void Library::saveFileNameInSettings(const QString &fileName) const
{
    QSettings settings;
    settings.setValue("library/databasefilename", fileName);
}

QString Library::fileNameFromArguments() const
{
    QStringList arguments = QApplication::arguments();
    arguments.removeFirst(); // appname

    if(arguments.size() < 1)
        return QString();

    QString databaseFilePath;
    foreach(QString a, arguments) {
        if(a == "-C")
            continue;

        databaseFilePath = a;
    }

    if(databaseFilePath.isEmpty())
        return QString();

    // An explicitly specified file has to exists
    QFile dbFile(databaseFilePath);
    if(!dbFile.exists()) {
        qWarning() << "The file does not exist:";
        qWarning() << databaseFilePath;
        QApplication::exit(-1);
    }

    return databaseFilePath;
}

QString Library::fileNameFromSettings() const
{
    QSettings settings;
    QString databaseFilePath = settings.value("library/databasefilename").toString();

    QFile dbFile(databaseFilePath);
    if(!dbFile.exists())
        return QString();

    return databaseFilePath;
}

QString Library::fileNameInDropbox() const
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QDir dataDir(dataPath);
    if(!dataDir.cd("Dropbox"))
        return QString();

    QString dirName = QApplication::applicationName();

    QString databaseFilePath = dataDir.absoluteFilePath(dirName + "/database.sqlite");
    if(QFile(databaseFilePath).exists())
        return databaseFilePath;


    QPixmap dropboxPixmap(":/general/dropbox");

    QMessageBox msg;
    msg.setWindowTitle(QObject::tr("Found your Dropbox!"));
    msg.setIconPixmap(dropboxPixmap);
    msg.setText(QObject::tr("Do you want to save your library in your Dropbox folder?"));
    msg.setInformativeText(QObject::tr("ProjectStats would save all games inside a new folder right inside your Dropbox."));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int answer = msg.exec();

    if(answer != QMessageBox::Yes)
        return QString();


    if(!dataDir.mkpath(dirName) || !dataDir.cd(dirName)) {
        qWarning() << "Could not create or cd to path:";
        qWarning() << dataPath;

        QMessageBox msg;
        msg.setWindowTitle(QObject::tr("Error"));
        msg.setText(QObject::tr("Could not create a new folder in your Dropbox!"));
        msg.setIconPixmap(dropboxPixmap);
        msg.setInformativeText(QObject::tr("%1").arg(dirName));
        msg.exec();

        return QString();
    }

    databaseFilePath = dataDir.absoluteFilePath("database.sqlite");

    if(!createFileIfNotExists(databaseFilePath)) {
        QMessageBox msg;
        msg.setWindowTitle(QObject::tr("Error"));
        msg.setIconPixmap(dropboxPixmap);
        msg.setText(QObject::tr("Could not create database in your Dropbox!"));
        msg.setInformativeText(QObject::tr("%1").arg(databaseFilePath));
        msg.exec();

        return QString();
    }

    return databaseFilePath;
}

QString Library::fileNameLocal() const
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dataDir(dataPath);

    if(!dataDir.mkpath(dataPath)) {
        qWarning() << "Could not create path:";
        qWarning() << dataPath;

        QMessageBox msg;
        msg.setWindowTitle(QObject::tr("Error"));
        msg.setText(QObject::tr("Could not create application data folder"));
        msg.setInformativeText(QObject::tr("%1").arg(dataPath));
        msg.exec();

        return QString();
    }

    QString databaseFilePath = dataDir.absoluteFilePath("database.sqlite");

    if(!createFileIfNotExists(databaseFilePath)) {
        QMessageBox msg;
        msg.setWindowTitle(QObject::tr("Error"));
        msg.setText(QObject::tr("Could not create database. You have a crappy filesystem, sir!"));
        msg.setInformativeText(QObject::tr("%1").arg(databaseFilePath));
        msg.exec();

        return QString();
    }

    return databaseFilePath;
}

bool Library::createFileIfNotExists(const QString &fileName) const
{
    QFile dbFile(fileName);
    if(!dbFile.exists()) {
        qDebug() << "Creating new file: " << fileName;
        if(!dbFile.open(QFile::WriteOnly)) {
            qWarning() << "Could not create file:";
            qWarning() << fileName;

            return false;
        }
        dbFile.close();
    }

    return true;
}


bool Library::setupPersistence()
{
    Qp::registerMappableTypes<int, int>();
    Qp::setDatabase(QSqlDatabase::database(DATABASE_CONNECTION_NAME));
    Qp::registerClass<LiveDrink>();
    Qp::registerClass<Place>();
    Qp::registerClass<Player>();
    Qp::registerClass<Drink>();
    Qp::registerClass<Game>();
    Qp::registerClass<Round>();
    Qp::registerClass<Schmeisserei>();
    Qp::registerClass<League>();
    Qp::registerClass<OLD_OfflineGameInformation>();
    //    Qp::registerClass<OLD_DokoOfflineGameBuddys>();
    Qp::adjustDatabaseSchema();

    // TODO: React to Qp::lastError, once it has been merged from geenen's branch
    // close the db if an error has occured!

    return true;
}

bool Library::fillCaches()
{
    m_players = Qp::readAll<Player>();
    m_games = Qp::readAll<Game>();
    m_rounds = Qp::readAll<Round>();
    return true;
}
