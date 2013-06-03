#include <QApplication>

#include <data/drink.h>
#include <data/game.h>
#include <data/livedrink.h>
#include <data/place.h>
#include <data/player.h>
#include <data/round.h>
#include <data/schmeisserei.h>
#include <data/league.h>

#include <ui/mainwindow.h>

#include <QSqlDatabase>
#include <QPersistence.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontShowIconsInMenus, true);
    a.setApplicationName("psnewmodel");
    a.setOrganizationName("lbproductions");

    if(a.arguments().size() != 2) {
        qDebug() << "Usage: psnewmodel <sqlite_database>";
        return 0;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(a.arguments().at(1));
    db.open();

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
    Qp::adjustDatabaseSchema();

    QList<QSharedPointer<Player> > players = Qp::readAll<Player>();
    QList<QSharedPointer<Game> > games = Qp::readAll<Game>();
    QList<QSharedPointer<Round> > rounds = Qp::readAll<Round>();

    MainWindow w;
    return a.exec();
}
