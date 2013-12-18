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

#include <ui/mainwindow.h>
#include <ui/widgets/popupwidget.h>
#include <ui/game/gamewindow.h>
#include <ui/startwindow.h>

#include <QSqlDatabase>
#include <QPersistence.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontShowIconsInMenus, true);
    a.setApplicationName("psnewmodel");
    a.setOrganizationName("lbproductions");

    /*
    if(a.arguments().size() != 2) {
        qDebug() << "Usage: psnewmodel <sqlite_database>";
        return 0;
    }
    */

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(a.arguments().at(1));
    db.setDatabaseName("/Users/niclasraabe/Dropbox/Public/projectstatsNewDB.db");
    //db.setDatabaseName("projectstatsNewDB.db");
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
    Qp::registerClass<OLD_OfflineGameInformation>();
    Qp::registerClass<OLD_DokoOfflineGameBuddys>();
    Qp::adjustDatabaseSchema();

    QList<QSharedPointer<Player> > players = Qp::readAll<Player>();
    QList<QSharedPointer<Game> > games = Qp::readAll<Game>();
    QList<QSharedPointer<Round> > rounds = Qp::readAll<Round>();

    /*
    foreach(QSharedPointer<Game> game, games) {
        if(game->type() == Game::Doppelkopf) {
            foreach(QSharedPointer<Round> round, game->rounds()) {
                if(Qp::primaryKey(round)>=3509) {
                    if(round->state() == Round::Finished && !round->contra1Player() && round->re1Player()) {
                        int contraPlayers = 0;
                        foreach(QSharedPointer<Player> player, round->playingPlayers()) {
                            if(!round->isRe(player)) {
                                switch(contraPlayers){
                                    case 0:
                                        round->setContra1Player(player);
                                        break;
                                    case 1:
                                        round->setContra2Player(player);
                                        break;
                                    case 2:
                                        round->setContra3Player(player);
                                        break;
                                }
                                contraPlayers++;
                            }
                        }
                        Qp::update(round);
                    }
                }
            }
        }
    }
    */
    //qDebug() << "Contra-Count: " << count;


    StartWindow* startWindow = new StartWindow();
    startWindow->show();

    //    new MainWindow;
    return a.exec();
}
