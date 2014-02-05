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
    QStringList arguments = QApplication::arguments();

    QString databaseFilePath;
    if(arguments.size() == 2) {
        databaseFilePath = arguments.at(1);

        // An explicitly specified file has to exists
        QFile dbFile(databaseFilePath);
        if(!dbFile.exists()) {
            qWarning() << "The database does not exist:";
            qWarning() << databaseFilePath;
            return false;
        }
    }

    // Fallback to local file in QStandardPaths::DataLocation
    if(databaseFilePath.isEmpty()) {
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
        QDir dataDir(dataPath);
        if(!dataDir.mkpath(dataPath)) {
            qWarning() << "Could not create path:";
            qWarning() << dataPath;
            return false;
        }

        databaseFilePath = dataDir.absoluteFilePath("database.sqlite");

        QFile dbFile(databaseFilePath);
        if(!dbFile.exists()) {
            qDebug() << "Creating new database: " << databaseFilePath;
            if(!dbFile.open(QFile::WriteOnly)) {
                qWarning() << "Could not create database:";
                qWarning() << databaseFilePath;
                return false;
            }
            dbFile.close();
        }
    }

    qDebug() << "Using database: ";
    qDebug() << databaseFilePath;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", DATABASE_CONNECTION_NAME);
    db.setDatabaseName(databaseFilePath);
    if(!db.open()) {
        qDebug() << "Could not open database!";
        return false;
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
