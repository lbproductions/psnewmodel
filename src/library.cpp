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
#include <QProcess>
#include <QPushButton>

#include <QSqlQuery>
#include <QSqlError>

namespace {
    static const QString DATABASE_CONNECTION_NAME("persistence");
}

Library::Library() :
    m_isOpen(false)
{
}

Library *Library::instance()
{
    static Library library;
    return &library;
}

bool Library::open()
{
    if(!setupDatabase())
        return false;

    if(!setupPersistence())
        return false;

    if(!fillCaches())
        return false;

    m_isOpen = true;
    return true;
}

void Library::close()
{
    if(Qp::database().isOpen())
        Qp::database().close();

    QFile file(fileNameFromSettings()+ ".lock");

    if(!file.exists())
        return;

    if(!file.remove())
        qWarning() << "Could not remove lock!";

}

QString Library::fileName() const
{
    return m_fileName;
}

QString Library::fileExtension()
{
    return QString(".psdb");
}

QString Library::defaultFileName()
{
    return QString("database" + fileExtension());
}

void Library::restartAndOpenLibrary(const QString &fileName)
{
    instance()->close();
    instance()->setFileName(fileName);

    QProcess::startDetached(QApplication::applicationFilePath(), QStringList() << fileName);
    QApplication::quit();
}

bool Library::setupDatabase()
{
    if(m_fileName.isEmpty())
        return false;

    qDebug() << "Using database: ";
    qDebug() << m_fileName;

    if(!lockDatabase(m_fileName)) {
        qDebug() << "Database is locked";
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", DATABASE_CONNECTION_NAME);
    db.setDatabaseName(m_fileName);

    if(!db.open()) {
        qDebug() << "Could not open database:";
        qDebug() << db.lastError();
        return false;
    }

    if(!fixDatabase(db)) {
        return false;
    }

    return true;
}

bool Library::lockDatabase(const QString &databaseFilePath)
{
    QFile file(databaseFilePath+ ".lock");

    if(!file.exists()) {
        if(file.open(QFile::WriteOnly)) {
            file.close();
            return true;
        }
    }

    qDebug() << "Database is locked!";

    QMessageBox msg;
    msg.setText(QObject::tr("Your library is locked"));
    msg.setIcon(QMessageBox::Warning);
    msg.setInformativeText(QObject::tr("Someone else might be using the library. You can unlock and proceed at the risk of running into problems later. If this is no shared library, it should be safe to continue."));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.button(QMessageBox::Yes)->setText(QObject::tr("Unlock"));
    msg.button(QMessageBox::No)->setText(QObject::tr("Quit"));
    int answer = msg.exec();

    if(answer == QMessageBox::No)
        return false;

    Q_ASSERT(answer == QMessageBox::Yes);
    if(!file.remove()) {
        qWarning() << "Could not remove lock";
        return false;
    }
    return true;
}

void Library::setFileName(const QString &fileName)
{
    m_fileName = fileName;
    QSettings settings;
    settings.setValue("library/databasefilename", fileName);
}

QString Library::packagePath() const
{
    return m_packagePath;
}

void Library::setPackagePath(const QString &packagePath)
{
    m_packagePath = packagePath;
}

QString Library::fileNameFromArguments()
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

QString Library::fileNameFromSettings()
{
    QSettings settings;
    QString databaseFilePath = settings.value("library/databasefilename").toString();

    QFile dbFile(databaseFilePath);
    if(!dbFile.exists())
        return QString();

    return databaseFilePath;
}

QString Library::fileNameLocal()
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

    if(!dataDir.cd("projectstats.pspkg")) {
        dataDir.mkdir("projectstats.pspkg");
        dataDir.cd("projectstats.pspkg");
    }

    if(!dataDir.cd("database")) {
        dataDir.mkdir("database");
        dataDir.cd("database");
    }

    QString databaseFilePath = dataDir.absoluteFilePath(defaultFileName());

    if(!createFileIfNotExists(databaseFilePath)) {
        QMessageBox msg;
        msg.setWindowTitle(QObject::tr("Error"));
        msg.setText(QObject::tr("Could not create database. You have a crappy filesystem, sir!"));
        msg.setInformativeText(QObject::tr("%1").arg(databaseFilePath));
        msg.exec();

        return QString();
    }

    return dataPath+"/projectstats.pspkg";
}

bool Library::createFileIfNotExists(const QString &fileName)
{
    QFile dbFile(fileName);
    if(!dbFile.exists()) {
        qDebug() << "Creating new file: " << fileName;
        if(!dbFile.open(QFile::WriteOnly)) {
            qWarning() << "Could not create file:";
            qWarning() << fileName;

            QMessageBox msg;
            msg.setText(QObject::tr("Could not create file '%1'!")
                        .arg(fileName));
            msg.setIcon(QMessageBox::Critical);
            msg.exec();

            return false;
        }
        dbFile.close();
    }

    return true;
}
bool Library::isOpen() const
{
    return m_isOpen;
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
    m_liveDrinks = Qp::readAll<LiveDrink>();
    return true;
}

bool Library::fixDatabase(QSqlDatabase db)
{
    if(!fixUniqueKeys(db, "game", "league"))
        return false;

    if(!fixUniqueKeys(db, "game", "player"))
        return false;

    if(!fixUniqueKeys(db, "league", "player"))
        return false;

    if(!fixUniqueKeys(db, "place", "player"))
        return false;

    return true;
}

bool Library::fixUniqueKeys(QSqlDatabase db, const QString &fk1, const QString &fk2)
{
    db.transaction();
    QSqlQuery query(db);
    QString table = QString("_Qp_REL_%1_%2s__%2_%1s")
                    .arg(fk1)
                    .arg(fk2);

    qDebug() << QString("Fixing unique key in table '%1'").arg(table);

    query.exec(QString("SELECT sql FROM sqlite_master WHERE name = '%1'")
               .arg(table));

    qDebug() << query.executedQuery();

    query.first();
    if(query.lastError().isValid()) {
        qDebug() << query.lastError();
        db.rollback();
        return false;
    }

    if(query.value(0).toString().contains("UNIQUE")) {
        qDebug() << "Table already fixed. Doing nothing.";
        return true;
    }

    query.clear();
    query.exec(QString("CREATE TEMP TABLE BACKUP_%1 "
                       "AS SELECT * FROM %1")
               .arg(table));
    qDebug() << query.executedQuery();

    if(query.lastError().isValid()) {
        qDebug() << query.lastError();
        db.rollback();
        return false;
    }


    query.clear();
    query.exec(QString("DROP TABLE %1")
               .arg(table));
    qDebug() << query.executedQuery();

    if(query.lastError().isValid()) {
        qDebug() << query.lastError();
        db.rollback();
        return false;
    }

    query.clear();
    query.exec(QString("CREATE TABLE %1 ("
                       "_Qp_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "_Qp_FK_%2 INTEGER,"
                       "_Qp_FK_%3 INTEGER,"
                       "FOREIGN KEY (_Qp_FK_%2) REFERENCES %2(_Qp_ID),"
                       "FOREIGN KEY (_Qp_FK_%3) REFERENCES %3(_Qp_ID),"
                       "UNIQUE (_Qp_FK_%2, _Qp_FK_%3)"
                       ")")
               .arg(table)
               .arg(fk1)
               .arg(fk2));
    qDebug() << query.executedQuery();

    if(query.lastError().isValid()) {
        qDebug() << query.lastError();
        db.rollback();
        return false;
    }

    query.clear();
    query.exec(QString("INSERT INTO %1 (_Qp_ID, _Qp_FK_%2, _Qp_FK_%3) "
                       "SELECT _Qp_ID, _Qp_FK_%2, _Qp_FK_%3 FROM BACKUP_%1")
               .arg(table)
               .arg(fk1)
               .arg(fk2));
    qDebug() << query.executedQuery();

    if(query.lastError().isValid()) {
        qDebug() << query.lastError();
        db.rollback();
        return false;
    }

    query.clear();
    query.exec(QString("DROP TABLE BACKUP_%1")
               .arg(table));
    qDebug() << query.executedQuery();

    if(query.lastError().isValid()) {
        qDebug() << query.lastError();
        db.rollback();
        return false;
    }

    db.commit();
    return true;
}
