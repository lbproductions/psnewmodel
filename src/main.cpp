#include <QApplication>

#include <data/drink.h>
#include <data/game.h>
#include <data/livedrink.h>
#include <data/place.h>
#include <data/player.h>
#include <data/round.h>
#include <data/schmeisserei.h>
#include <data/league.h>
#include <data/old_offlineGameInformation.h>
#include <data/old_dokoofflinegamebuddys.h>

#include <misc/updater/updater.h>
#include <misc/cocoainitializer.h>

#include <ui/mainwindow.h>
#include <ui/widgets/popupwidget.h>
#include <ui/game/gamewindow.h>
#include <ui/startwindow.h>

#include <QPersistence.h>

#include <QSqlDatabase>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontShowIconsInMenus, true);
    a.setApplicationName("psnewmodel");
    a.setOrganizationName("LB Productions");
    a.setOrganizationDomain("lbproductions.github.com");

    CocoaInitializer cocoaInitializer;
    Q_UNUSED(cocoaInitializer);

    Updater::instanceForPlatform()->checkForUpdatesInBackground();

    QString databaseFilePath;
    if(a.arguments().size() == 2) {
        databaseFilePath = a.arguments().at(1);

        // An explicitly specified file has to exists
        QFile dbFile(databaseFilePath);
        if(!dbFile.exists()) {
            qWarning() << "The database does not exist:";
            qWarning() << databaseFilePath;
            return 0;
        }
    }
    else {
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
        QDir dataDir(dataPath);
        if(!dataDir.mkpath(dataPath)) {
            qWarning() << "Could not create path:";
            qWarning() << dataPath;
            return 0;
        }

        databaseFilePath = dataDir.absoluteFilePath("database.sqlite");

        QFile dbFile(databaseFilePath);
        if(!dbFile.exists()) {
            qDebug() << "Creating new database: " << databaseFilePath;
            if(!dbFile.open(QFile::WriteOnly)) {
                qWarning() << "Could not create database:";
                qWarning() << databaseFilePath;
                return 0;
            }
            dbFile.close();
        }
    }

    qDebug() << "Using database: ";
    qDebug() << databaseFilePath;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseFilePath);
    if(!db.open()) {
        qDebug() << "Could not open database!";
        return 0;
    }

    Qp::registerMappableTypes<int, int>();
    Qp::setDatabase(db);
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

    QList<QSharedPointer<Player> > players = Qp::readAll<Player>();
    QList<QSharedPointer<Game> > games = Qp::readAll<Game>();
    QList<QSharedPointer<Round> > rounds = Qp::readAll<Round>();
    QList<QSharedPointer<LiveDrink> > liveDrinks = Qp::readAll<LiveDrink>();

    StartWindow* startWindow = new StartWindow();
    startWindow->show();

    int ret = a.exec();

    delete Updater::instanceForPlatform();
    delete startWindow;
    Qp::database().close();

    return ret;
}
