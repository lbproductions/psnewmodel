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

#include <QSqlRecord>
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
    QSettings settings;
    settings.setValue("library/packagePath", packagePath);
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
    QString databaseFilePath = settings.value("library/packagePath").toString();

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

    if(Qp::lastError().isValid()) {
        qDebug() << Qp::lastError();
        return false;
    }

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
    QSqlQuery query(db);
    query.exec(QString("SELECT sql FROM sqlite_master WHERE name = '_Qp_REL_game_players__player_games'"));
    query.first();
    if(query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    if(query.value(0).toString().contains("UNIQUE")) {
        qDebug() << "Database already fixed.";
        return true;
    }
    query.finish();

    if(!fixGamePositions(db))
        return false;

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

bool Library::fixGamePositions(QSqlDatabase db)
{
    fixGamePositionsFromOldDb(db);
    fixGamePositionsFromOldDb(db, "projectstatsNewDB.db", 358, 381);

    QSqlQuery query(db);
    query.exec("DELETE FROM _qp_rel_game_players__player_games WHERE NOT EXISTS "
               "(SELECT 1 FROM game WHERE game._qp_id = _qp_rel_game_players__player_games._qp_fk_game)");
    qDebug() << query.executedQuery();

    query.clear();
    query.exec("SELECT * FROM _Qp_REL_game_players__player_games ORDER BY _Qp_ID ASC");


    QSqlQuery querySelectPlayerCount(db);
    querySelectPlayerCount.prepare("SELECT COUNT(*) FROM _Qp_REL_game_players__player_games WHERE _qp_fk_game = ?");

    int playerCount = 0;
    int expectedPlayerCount = -1;
    int currentGame = -1;
    QVariant position;
    while(query.next()) {
        int game = query.value("_qp_fk_game").toInt();

        if(game != currentGame && currentGame > 0) {
            if(game < currentGame) {
                continue;
            }

            if(expectedPlayerCount != playerCount) {
                if(position.isNull())
                    fixGamePositionsById(currentGame, expectedPlayerCount, db);
                else
                    fixGamePositionsByPosition(currentGame, expectedPlayerCount, db);
            }

            playerCount = 0;
            expectedPlayerCount = -1;
        }

        currentGame = game;

        if(expectedPlayerCount < 0) {
            querySelectPlayerCount.addBindValue(game);
            querySelectPlayerCount.exec();
            querySelectPlayerCount.first();
            expectedPlayerCount = querySelectPlayerCount.value(0).toInt();
        }

        position = query.value("position");

        ++playerCount;
    }

    return true;
}

bool Library::fixGamePositionsFromOldDb(QSqlDatabase db)
{
    QSqlDatabase olddb = QSqlDatabase::addDatabase("QSQLITE", "olddbconnection");
    olddb.setDatabaseName("/Users/niklaswulf/Downloads/projectstats.db");

    if(!olddb.open()) {
        qDebug() << "Could not open database:";
        qDebug() << olddb.lastError();
        return false;
    }

    QSqlQuery query(olddb);
    query.exec(QString("SELECT * FROM positions ORDER BY gameId ASC, position ASC"));
    qDebug() << query.executedQuery();

    if(query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    QSqlQuery deleteQuery(db);
    deleteQuery.exec(QString("DELETE FROM _Qp_REL_game_players__player_games WHERE _qp_fk_game <= 357"));
    qDebug() << deleteQuery.executedQuery();

    if(deleteQuery.lastError().isValid()) {
        qDebug() << deleteQuery.lastError();
        return false;
    }

    QSqlQuery insert(db);
    insert.prepare("INSERT INTO _Qp_REL_game_players__player_games (_qp_id, _qp_fk_game, _qp_fk_player) VALUES (?,?,?)");


    int id = 0;
    while(query.next()) {
        insert.addBindValue(++id);
        insert.addBindValue(query.value("gameId").toInt());
        insert.addBindValue(query.value("playerId").toInt());
        insert.exec();

        if(insert.lastError().isValid()) {
            qDebug() << insert.lastError();
            return false;
        }

        qDebug() << insert.executedQuery();
        qDebug() << insert.boundValues();
    }

    olddb.close();

    return true;
}

bool Library::fixGamePositionsFromOldDb(QSqlDatabase db, const QString &oldDb, int begin, int end)
{
    QSqlDatabase::removeDatabase("olddbconnection");
    QSqlDatabase olddb = QSqlDatabase::addDatabase("QSQLITE", "asd");
    olddb.setDatabaseName("/Users/niklaswulf/Dropbox/psnewmodel/Shared/Dortmund/Backups/"+oldDb);

    if(!olddb.open()) {
        qDebug() << "Could not open database:";
        qDebug() << olddb.lastError();
        return false;
    }

    QSqlQuery deleteQuery(db);
    deleteQuery.exec(QString("DELETE FROM _Qp_REL_game_players__player_games WHERE _qp_fk_game <= %1 AND _qp_fk_game >= %2")
                     .arg(end)
                     .arg(begin));
    qDebug() << deleteQuery.executedQuery();
    if(deleteQuery.lastError().isValid()) {
        qDebug() << deleteQuery.lastError();
        return false;
    }

    qDebug() << olddb.tables();

    QSqlQuery query(olddb);
    query.exec(QString("SELECT * FROM _Qp_REL_game_players__player_games WHERE _qp_fk_game <= %1 AND _qp_fk_game >= %2 ORDER BY _qp_id ASC")
               .arg(end)
               .arg(begin));
    qDebug() << query.executedQuery();
    if(query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    QSqlQuery insert(db);
    insert.prepare("INSERT INTO _Qp_REL_game_players__player_games (_qp_fk_game, _qp_fk_player) VALUES (?,?)");

    while(query.next()) {
        insert.addBindValue(query.value("_qp_fk_game").toInt());
        insert.addBindValue(query.value("_qp_fk_player").toInt());
        insert.exec();
        if(insert.lastError().isValid()) {
            qDebug() << insert.lastError();
            return false;
        }
        qDebug() << insert.executedQuery();
        qDebug() << insert.boundValues();
    }

    return true;
}

bool Library::fixGamePositionsByPosition(int game, int expectedPlayerCount, QSqlDatabase db)
{
    qDebug() << "FIXING" << game << "by position";

    QSqlQuery query(db);
    query.prepare("SELECT * FROM _Qp_REL_game_players__player_games WHERE _qp_fk_game = ? ORDER BY position ASC");
    query.addBindValue(game);
    query.exec();

    int previousPosition = -1;

    QList<int> missingPlayers;
    QList<int> missingPositions;

    while(query.next()) {
        QVariant position = query.value("position");
        if(position.toString() == "")
            continue;

        int pos = position.toInt();

        while(previousPosition + 1 < pos) {
            missingPositions << previousPosition + 1;
            ++previousPosition;
        }

        previousPosition = pos;
    }

    qDebug() << "Expecting " << expectedPlayerCount << "players";

    while(previousPosition + 1 < expectedPlayerCount) {
        missingPositions << previousPosition + 1;
        ++previousPosition;
    }

    qDebug() << "missing players" << missingPlayers;
    qDebug() << "missing positions" << missingPositions;

    return true;
}

bool Library::fixGamePositionsById(int game, int expectedPlayerCount, QSqlDatabase newDb)
{
    qDebug() << "FIX " << game;

    QSqlQuery query(newDb);
    query.prepare("SELECT * FROM _Qp_REL_game_players__player_games WHERE _qp_fk_game = ? ORDER BY _qp_id ASC");
    query.addBindValue(game);
    query.exec();

    int previousPosition = -1;
    int okayCount = 0;

    while(query.next()) {
        QVariant position = query.value("_qp_id");
        int pos = position.toInt();

        if(previousPosition < 0) {
            previousPosition = pos;
            ++okayCount;
            continue;
        }

        if(previousPosition + 8 < pos)
            break;

        ++okayCount;
        while(previousPosition + 1 < pos && previousPosition + 10 > pos) {
            ++previousPosition;
        }

        previousPosition = pos;
    }

    qDebug() << "Expecting " << expectedPlayerCount << "players, found " << okayCount;

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
    query.finish();
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
