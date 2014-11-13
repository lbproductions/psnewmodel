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

    QSharedPointer<Player> niklas = Qp::read<Player>(5);
    QSharedPointer<Player> niclas = Qp::read<Player>(3);
    QSharedPointer<Player> stephan = Qp::read<Player>(2);
    QSharedPointer<Player> uli = Qp::read<Player>(6);
    QSharedPointer<Player> patrick = Qp::read<Player>(7);
    QSharedPointer<Player> mirko = Qp::read<Player>(11);
    QSharedPointer<Player> johannes = Qp::read<Player>(12);
    QSharedPointer<Player> timon = Qp::read<Player>(45);

    QDateTime date(QDate(2014,11,12), QTime(18,30,0));
    int number = 0;

    QSharedPointer<Game> game = Qp::create<Game>();
    game->addPlayer(niklas);
    game->addPlayer(niclas);
    game->addPlayer(stephan);
    game->addPlayer(uli);
    game->addPlayer(patrick);
    game->addPlayer(mirko);
    game->addPlayer(timon);
    game->addPlayer(johannes);

    game->setSite(Qp::read<Place>(31));
    game->setMitPflichtSolo(true);
    game->setType(Game::Doppelkopf);
    game->setCreationTime(date);
    game->setState(Game::Finished);

    QSharedPointer<Round> round1 = Qp::create<Round>();
    game->addRound(round1);
    round1->setNumber(0);
    round1->setState(Round::Finished);
    date.addSecs(180);
    round1->setStartTime(date);
    round1->setRe1Player(johannes);
    round1->setRe2Player(timon);
    round1->setContra1Player(uli);
    round1->setContra2Player(mirko);
    Qp::update(round1);
    int reGamePoints = 95;
    round1->setReGamePoints(reGamePoints);
    round1->setContraGamePoints(240-reGamePoints);
    int rePoints = -4;
    round1->setPoints(round1->re1Player(), rePoints);
    round1->setPoints(round1->re2Player(), rePoints);
    round1->setPoints(round1->contra1Player(), -rePoints);
    round1->setPoints(round1->contra2Player(), -rePoints);
    round1->setHochzeitPlayer(johannes);
    round1->setHochzeitDecision(Round::ErsterFehl);
    round1->setSchweinereiPlayer(uli);
    Qp::update(round1);

    QSharedPointer<Round> round2 = Qp::create<Round>();
    game->addRound(round2);
    round2->setState(Round::Finished);
    date.addSecs(180);
    number++;
    round2->setNumber(number);
    round1->setStartTime(date);
    round2->setRe1Player(niklas);
    round2->setRe2Player(niclas);
    round2->setContra1Player(patrick);
    round2->setContra2Player(stephan);
    Qp::update(round2);
    reGamePoints = 141;
    round2->setReGamePoints(reGamePoints);
    round2->setContraGamePoints(240-reGamePoints);
    rePoints = 3;
    round2->setPoints(round2->re1Player(), rePoints);
    round2->setPoints(round2->re2Player(), rePoints);
    round2->setPoints(round2->contra1Player(), -rePoints);
    round2->setPoints(round2->contra2Player(), -rePoints);
    round2->setTrumpfabgabePlayer(niclas);
    round2->setTrumpfCount(3);
    round2->setTrumpfZurueck(0);
    Qp::update(round2);

    QSharedPointer<Round> round3 = Qp::create<Round>();
    game->addRound(round3);
    round3->setState(Round::Finished);
    number++;
    round3->setNumber(number);
    date.addSecs(180);
    round3->setStartTime(date);
    round3->setRe1Player(timon);
    round3->setRe2Player(stephan);
    round3->setContra1Player(niklas);
    round3->setContra2Player(patrick);
    Qp::update(round3);
    reGamePoints = 159;
    round3->setReGamePoints(reGamePoints);
    round3->setContraGamePoints(240-reGamePoints);
    rePoints = 4;
    round3->setPoints(round3->re1Player(), rePoints);
    round3->setPoints(round3->re2Player(), rePoints);
    round3->setPoints(round3->contra1Player(), -rePoints);
    round3->setPoints(round3->contra2Player(), -rePoints);
    Qp::update(round3);

    // SOLO
    QSharedPointer<Round> round4 = Qp::create<Round>();
    game->addRound(round4);
    round4->setState(Round::Finished);
    number++;
    round4->setNumber(number);
    date.addSecs(180);
    round4->setStartTime(date);
    round4->setRe1Player(johannes);
    round4->setContra1Player(mirko);
    round4->setContra2Player(uli);
    round4->setContra3Player(niclas);
    Qp::update(round4);
    reGamePoints = 124;
    round4->setReGamePoints(reGamePoints);
    round4->setContraGamePoints(240-reGamePoints);
    rePoints = 1;
    round4->setPoints(round4->re1Player(), rePoints*3);
    round4->setPoints(round4->contra1Player(), -rePoints);
    round4->setPoints(round4->contra2Player(), -rePoints);
    round4->setPoints(round4->contra3Player(), -rePoints);
    round4->setSoloPlayer(johannes);
    round4->setIsPflicht(true);
    round4->setSoloType(Round::BubenSolo);
    Qp::update(round4);

    QSharedPointer<Schmeisserei> schmeisserei1 = Qp::create<Schmeisserei>();
    schmeisserei1->setPlayer(uli);
    schmeisserei1->setType(Schmeisserei::MoreThan4Kings);
    schmeisserei1->setKingsCount(5);
    round4->addSchmeisserei(schmeisserei1);
    Qp::update(schmeisserei1);
    Qp::update(round4);

    QSharedPointer<Round> round5 = Qp::create<Round>();
    game->addRound(round5);
    round5->setState(Round::Finished);
    number++;
    round5->setNumber(number);
    round5->setStartTime(date);
    round5->setRe1Player(niclas);
    round5->setRe2Player(patrick);
    round5->setContra1Player(mirko);
    round5->setContra2Player(johannes);
    Qp::update(round5);
    reGamePoints = 165;
    round5->setReGamePoints(reGamePoints);
    round5->setContraGamePoints(240-reGamePoints);
    rePoints = 2;
    round5->setPoints(round5->re1Player(), rePoints);
    round5->setPoints(round5->re2Player(), rePoints);
    round5->setPoints(round5->contra1Player(), -rePoints);
    round5->setPoints(round5->contra2Player(), -rePoints);
    Qp::update(round5);

    QSharedPointer<Round> round6 = Qp::create<Round>();
    game->addRound(round6);
    round6->setState(Round::Finished);
    number++;
    round6->setNumber(number);
    date.addSecs(180);
    round6->setStartTime(date);
    round6->setRe1Player(uli);
    round6->setRe2Player(stephan);
    round6->setContra1Player(niklas);
    round6->setContra2Player(timon);
    Qp::update(round6);
    reGamePoints = 70;
    round6->setReGamePoints(reGamePoints);
    round6->setContraGamePoints(240-reGamePoints);
    rePoints = -5;
    round6->setPoints(round6->re1Player(), rePoints);
    round6->setPoints(round6->re2Player(), rePoints);
    round6->setPoints(round6->contra1Player(), -rePoints);
    round6->setPoints(round6->contra2Player(), -rePoints);
    Qp::update(round6);

    QSharedPointer<Round> round7 = Qp::create<Round>();
    game->addRound(round7);
    round7->setState(Round::Finished);
    number++;
    round7->setNumber(number);
    date.addSecs(180);
    round7->setStartTime(date);
    round7->setRe1Player(stephan);
    round7->setRe2Player(uli);
    round7->setContra1Player(niklas);
    round7->setContra2Player(mirko);
    Qp::update(round7);
    reGamePoints = 174;
    round7->setReGamePoints(reGamePoints);
    round7->setContraGamePoints(240-reGamePoints);
    rePoints = 4;
    round7->setPoints(round7->re1Player(), rePoints);
    round7->setPoints(round7->re2Player(), rePoints);
    round7->setPoints(round7->contra1Player(), -rePoints);
    round7->setPoints(round7->contra2Player(), -rePoints);
    Qp::update(round7);

    QSharedPointer<Round> round8 = Qp::create<Round>();
    game->addRound(round8);
    round8->setState(Round::Finished);
    number++;
    round8->setNumber(number);
    date.addSecs(180);
    round8->setStartTime(date);
    round8->setRe1Player(timon);
    round8->setRe2Player(niclas);
    round8->setContra1Player(patrick);
    round8->setContra2Player(johannes);
    Qp::update(round8);
    reGamePoints = 240;
    round8->setReGamePoints(reGamePoints);
    round8->setContraGamePoints(240-reGamePoints);
    rePoints = 10;
    round8->setPoints(round8->re1Player(), rePoints);
    round8->setPoints(round8->re2Player(), rePoints);
    round8->setPoints(round8->contra1Player(), -rePoints);
    round8->setPoints(round8->contra2Player(), -rePoints);
    round8->setHochzeitPlayer(timon);
    round8->setHochzeitDecision(Round::ErsterFehl);
    Qp::update(round8);

    QSharedPointer<Round> round9 = Qp::create<Round>();
    game->addRound(round9);
    round9->setState(Round::Finished);
    number++;
    round9->setNumber(number);
    date.addSecs(180);
    round9->setStartTime(date);
    round9->setRe1Player(timon);
    round9->setRe2Player(johannes);
    round9->setContra1Player(stephan);
    round9->setContra2Player(niclas);
    Qp::update(round9);
    reGamePoints = 112;
    round9->setReGamePoints(reGamePoints);
    round9->setContraGamePoints(240-reGamePoints);
    rePoints = -2;
    round9->setPoints(round9->re1Player(), rePoints);
    round9->setPoints(round9->re2Player(), rePoints);
    round9->setPoints(round9->contra1Player(), -rePoints);
    round9->setPoints(round9->contra2Player(), -rePoints);
    Qp::update(round9);

    // SOLO
    QSharedPointer<Round> round10 = Qp::create<Round>();
    game->addRound(round10);
    round10->setState(Round::Finished);
    number++;
    round10->setNumber(number);
    date.addSecs(180);
    round10->setStartTime(date);
    round10->setRe1Player(mirko);
    round10->setContra1Player(patrick);
    round10->setContra2Player(uli);
    round10->setContra3Player(niklas);
    Qp::update(round10);
    reGamePoints = 181;
    round10->setReGamePoints(reGamePoints);
    round10->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round10->setPoints(round10->re1Player(), rePoints*3);
    round10->setPoints(round10->contra1Player(), -rePoints);
    round10->setPoints(round10->contra2Player(), -rePoints);
    round10->setPoints(round10->contra3Player(), -rePoints);
    round10->setSoloPlayer(mirko);
    round10->setIsPflicht(true);
    round10->setSoloType(Round::BubenSolo);
    Qp::update(round10);

    QSharedPointer<Round> round11 = Qp::create<Round>();
    game->addRound(round11);
    round11->setState(Round::Finished);
    number++;
    round11->setNumber(number);
    date.addSecs(180);
    round11->setStartTime(date);
    round11->setRe1Player(niklas);
    round11->setRe2Player(uli);
    round11->setContra1Player(patrick);
    round11->setContra2Player(johannes);
    Qp::update(round11);
    reGamePoints = 138;
    round11->setReGamePoints(reGamePoints);
    round11->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round11->setPoints(round11->re1Player(), rePoints);
    round11->setPoints(round11->re2Player(), rePoints);
    round11->setPoints(round11->contra1Player(), -rePoints);
    round11->setPoints(round11->contra2Player(), -rePoints);
    round11->setHochzeitPlayer(niklas);
    round11->setHochzeitDecision(Round::ErsterFehl);
    Qp::update(round11);

    QSharedPointer<Round> round12 = Qp::create<Round>();
    game->addRound(round12);
    round12->setState(Round::Finished);
    date.addSecs(180);
    number++;
    round12->setNumber(number);
    round1->setStartTime(date);
    round12->setRe1Player(stephan);
    round12->setRe2Player(timon);
    round12->setContra1Player(niclas);
    round12->setContra2Player(mirko);
    Qp::update(round12);
    reGamePoints = 162;
    round12->setReGamePoints(reGamePoints);
    round12->setContraGamePoints(240-reGamePoints);
    rePoints = 4;
    round12->setPoints(round12->re1Player(), rePoints);
    round12->setPoints(round12->re2Player(), rePoints);
    round12->setPoints(round12->contra1Player(), -rePoints);
    round12->setPoints(round12->contra2Player(), -rePoints);
    round12->setTrumpfabgabePlayer(stephan);
    round12->setTrumpfCount(3);
    round12->setTrumpfZurueck(2);
    Qp::update(round12);

    QSharedPointer<Round> round13 = Qp::create<Round>();
    game->addRound(round13);
    round13->setState(Round::Finished);
    number++;
    round13->setNumber(number);
    date.addSecs(180);
    round13->setStartTime(date);
    round13->setRe1Player(mirko);
    round13->setRe2Player(niclas);
    round13->setContra1Player(niklas);
    round13->setContra2Player(timon);
    Qp::update(round13);
    reGamePoints = 195;
    round13->setReGamePoints(reGamePoints);
    round13->setContraGamePoints(240-reGamePoints);
    rePoints = 6;
    round13->setPoints(round13->re1Player(), rePoints);
    round13->setPoints(round13->re2Player(), rePoints);
    round13->setPoints(round13->contra1Player(), -rePoints);
    round13->setPoints(round13->contra2Player(), -rePoints);
    round13->setHochzeitPlayer(mirko);
    round13->setHochzeitDecision(Round::ErsterFehl);
    Qp::update(round13);

    QSharedPointer<Round> round14 = Qp::create<Round>();
    game->addRound(round14);
    round14->setState(Round::Finished);
    number++;
    round14->setNumber(number);
    date.addSecs(180);
    round14->setStartTime(date);
    round14->setRe1Player(uli);
    round14->setRe2Player(patrick);
    round14->setContra1Player(stephan);
    round14->setContra2Player(johannes);
    Qp::update(round14);
    reGamePoints = 86;
    round14->setReGamePoints(reGamePoints);
    round14->setContraGamePoints(240-reGamePoints);
    rePoints = -7;
    round14->setPoints(round14->re1Player(), rePoints);
    round14->setPoints(round14->re2Player(), rePoints);
    round14->setPoints(round14->contra1Player(), -rePoints);
    round14->setPoints(round14->contra2Player(), -rePoints);
    Qp::update(round14);

    // SOLO
    QSharedPointer<Round> round15 = Qp::create<Round>();
    game->addRound(round15);
    round15->setState(Round::Finished);
    number++;
    round15->setNumber(number);
    date.addSecs(180);
    round15->setStartTime(date);
    round15->setRe1Player(stephan);
    round15->setContra1Player(patrick);
    round15->setContra2Player(uli);
    round15->setContra3Player(niclas);
    Qp::update(round15);
    reGamePoints = 188;
    round15->setReGamePoints(reGamePoints);
    round15->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round15->setPoints(round15->re1Player(), rePoints*3);
    round15->setPoints(round15->contra1Player(), -rePoints);
    round15->setPoints(round15->contra2Player(), -rePoints);
    round15->setPoints(round15->contra3Player(), -rePoints);
    round15->setSoloPlayer(stephan);
    round15->setIsPflicht(true);
    round15->setSoloType(Round::BubenSolo);
    Qp::update(round15);

    QSharedPointer<Round> round16 = Qp::create<Round>();
    game->addRound(round16);
    round16->setState(Round::Finished);
    number++;
    round16->setNumber(number);
    date.addSecs(180);
    round16->setStartTime(date);
    round16->setRe1Player(mirko);
    round16->setRe2Player(timon);
    round16->setContra1Player(niklas);
    round16->setContra2Player(johannes);
    Qp::update(round16);
    reGamePoints = 139;
    round16->setReGamePoints(reGamePoints);
    round16->setContraGamePoints(240-reGamePoints);
    rePoints = 1;
    round16->setPoints(round16->re1Player(), rePoints);
    round16->setPoints(round16->re2Player(), rePoints);
    round16->setPoints(round16->contra1Player(), -rePoints);
    round16->setPoints(round16->contra2Player(), -rePoints);
    Qp::update(round16);

    QSharedPointer<Round> round17 = Qp::create<Round>();
    game->addRound(round17);
    round17->setState(Round::Finished);
    number++;
    round17->setNumber(number);
    date.addSecs(180);
    round17->setStartTime(date);
    round17->setRe1Player(timon);
    round17->setRe2Player(patrick);
    round17->setContra1Player(mirko);
    round17->setContra2Player(johannes);
    Qp::update(round17);
    reGamePoints = 182;
    round17->setReGamePoints(reGamePoints);
    round17->setContraGamePoints(240-reGamePoints);
    rePoints = 6;
    round17->setPoints(round17->re1Player(), rePoints);
    round17->setPoints(round17->re2Player(), rePoints);
    round17->setPoints(round17->contra1Player(), -rePoints);
    round17->setPoints(round17->contra2Player(), -rePoints);
    Qp::update(round17);

    QSharedPointer<Round> round18 = Qp::create<Round>();
    game->addRound(round18);
    round18->setState(Round::Finished);
    number++;
    round18->setNumber(number);
    date.addSecs(180);
    round18->setStartTime(date);
    round18->setRe1Player(niclas);
    round18->setRe2Player(uli);
    round18->setContra1Player(niklas);
    round18->setContra2Player(stephan);
    Qp::update(round18);
    reGamePoints = 137;
    round18->setReGamePoints(reGamePoints);
    round18->setContraGamePoints(240-reGamePoints);
    rePoints = 3;
    round18->setPoints(round18->re1Player(), rePoints);
    round18->setPoints(round18->re2Player(), rePoints);
    round18->setPoints(round18->contra1Player(), -rePoints);
    round18->setPoints(round18->contra2Player(), -rePoints);
    round18->setSchweinereiPlayer(niklas);
    Qp::update(round18);

    QSharedPointer<Round> round19 = Qp::create<Round>();
    game->addRound(round19);
    round19->setState(Round::Finished);
    number++;
    round19->setNumber(number);
    date.addSecs(180);
    round19->setStartTime(date);
    round19->setRe1Player(patrick);
    round19->setRe2Player(stephan);
    round19->setContra1Player(mirko);
    round19->setContra2Player(niklas);
    Qp::update(round19);
    reGamePoints = 80;
    round19->setReGamePoints(reGamePoints);
    round19->setContraGamePoints(240-reGamePoints);
    rePoints = -7;
    round19->setPoints(round19->re1Player(), rePoints);
    round19->setPoints(round19->re2Player(), rePoints);
    round19->setPoints(round19->contra1Player(), -rePoints);
    round19->setPoints(round19->contra2Player(), -rePoints);
    Qp::update(round19);

    QSharedPointer<Round> round20 = Qp::create<Round>();
    game->addRound(round20);
    round20->setState(Round::Finished);
    number++;
    round20->setNumber(number);
    date.addSecs(180);
    round20->setStartTime(date);
    round20->setRe1Player(uli);
    round20->setRe2Player(timon);
    round20->setContra1Player(niclas);
    round20->setContra2Player(johannes);
    Qp::update(round20);
    reGamePoints = 114;
    round20->setReGamePoints(reGamePoints);
    round20->setContraGamePoints(240-reGamePoints);
    rePoints = -3;
    round20->setPoints(round20->re1Player(), rePoints);
    round20->setPoints(round20->re2Player(), rePoints);
    round20->setPoints(round20->contra1Player(), -rePoints);
    round20->setPoints(round20->contra2Player(), -rePoints);
    Qp::update(round20);

    QSharedPointer<Round> round21 = Qp::create<Round>();
    game->addRound(round21);
    round21->setState(Round::Finished);
    number++;
    round21->setNumber(number);
    date.addSecs(180);
    round21->setStartTime(date);
    round21->setRe1Player(niclas);
    round21->setRe2Player(stephan);
    round21->setContra1Player(johannes);
    round21->setContra2Player(mirko);
    Qp::update(round21);
    reGamePoints = 194;
    round21->setReGamePoints(reGamePoints);
    round21->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round21->setPoints(round21->re1Player(), rePoints);
    round21->setPoints(round21->re2Player(), rePoints);
    round21->setPoints(round21->contra1Player(), -rePoints);
    round21->setPoints(round21->contra2Player(), -rePoints);
    Qp::update(round21);

    // SOLO
    QSharedPointer<Round> round22 = Qp::create<Round>();
    game->addRound(round22);
    round22->setState(Round::Finished);
    number++;
    round22->setNumber(number);
    date.addSecs(180);
    round22->setStartTime(date);
    round22->setRe1Player(niklas);
    round22->setContra1Player(uli);
    round22->setContra2Player(timon);
    round22->setContra3Player(patrick);
    Qp::update(round22);
    reGamePoints = 116;
    round22->setReGamePoints(reGamePoints);
    round22->setContraGamePoints(240-reGamePoints);
    rePoints = -3;
    round22->setPoints(round22->re1Player(), rePoints*3);
    round22->setPoints(round22->contra1Player(), -rePoints);
    round22->setPoints(round22->contra2Player(), -rePoints);
    round22->setPoints(round22->contra3Player(), -rePoints);
    round22->setSoloPlayer(niklas);
    round22->setIsPflicht(true);
    round22->setSoloType(Round::DamenSolo);
    Qp::update(round22);

    QSharedPointer<Round> round23 = Qp::create<Round>();
    game->addRound(round23);
    round23->setState(Round::Finished);
    number++;
    round23->setNumber(number);
    date.addSecs(180);
    round23->setStartTime(date);
    round23->setRe1Player(niklas);
    round23->setRe2Player(stephan);
    round23->setContra1Player(johannes);
    round23->setContra2Player(uli);
    Qp::update(round23);
    reGamePoints = 201;
    round23->setReGamePoints(reGamePoints);
    round23->setContraGamePoints(240-reGamePoints);
    rePoints = 8;
    round23->setPoints(round23->re1Player(), rePoints);
    round23->setPoints(round23->re2Player(), rePoints);
    round23->setPoints(round23->contra1Player(), -rePoints);
    round23->setPoints(round23->contra2Player(), -rePoints);
    Qp::update(round23);

    // SOLO
    QSharedPointer<Round> round24 = Qp::create<Round>();
    game->addRound(round24);
    round24->setState(Round::Finished);
    number++;
    round24->setNumber(number);
    date.addSecs(180);
    round24->setStartTime(date);
    round24->setRe1Player(patrick);
    round24->setContra1Player(mirko);
    round24->setContra2Player(timon);
    round24->setContra3Player(niclas);
    Qp::update(round24);
    reGamePoints = 113;
    round24->setReGamePoints(reGamePoints);
    round24->setContraGamePoints(240-reGamePoints);
    rePoints = -3;
    round24->setPoints(round24->re1Player(), rePoints*3);
    round24->setPoints(round24->contra1Player(), -rePoints);
    round24->setPoints(round24->contra2Player(), -rePoints);
    round24->setPoints(round24->contra3Player(), -rePoints);
    round24->setSoloPlayer(patrick);
    round24->setIsPflicht(true);
    round24->setSoloType(Round::DamenSolo);
    Qp::update(round24);

    QSharedPointer<Round> round25 = Qp::create<Round>();
    game->addRound(round25);
    round25->setState(Round::Finished);
    date.addSecs(180);
    number++;
    round25->setNumber(number);
    round1->setStartTime(date);
    round25->setRe1Player(niklas);
    round25->setRe2Player(niclas);
    round25->setContra1Player(timon);
    round25->setContra2Player(johannes);
    Qp::update(round25);
    reGamePoints = 150;
    round25->setReGamePoints(reGamePoints);
    round25->setContraGamePoints(240-reGamePoints);
    rePoints = 3;
    round25->setPoints(round25->re1Player(), rePoints);
    round25->setPoints(round25->re2Player(), rePoints);
    round25->setPoints(round25->contra1Player(), -rePoints);
    round25->setPoints(round25->contra2Player(), -rePoints);
    round25->setTrumpfabgabePlayer(niklas);
    round25->setTrumpfCount(3);
    round25->setTrumpfZurueck(0);
    Qp::update(round25);

    QSharedPointer<Round> round26 = Qp::create<Round>();
    game->addRound(round26);
    round26->setState(Round::Finished);
    number++;
    round26->setNumber(number);
    date.addSecs(180);
    round26->setStartTime(date);
    round26->setRe1Player(patrick);
    round26->setRe2Player(mirko);
    round26->setContra1Player(stephan);
    round26->setContra2Player(uli);
    Qp::update(round26);
    reGamePoints = 168;
    round26->setReGamePoints(reGamePoints);
    round26->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round26->setPoints(round26->re1Player(), rePoints);
    round26->setPoints(round26->re2Player(), rePoints);
    round26->setPoints(round26->contra1Player(), -rePoints);
    round26->setPoints(round26->contra2Player(), -rePoints);
    Qp::update(round26);

    QSharedPointer<Round> round27 = Qp::create<Round>();
    game->addRound(round27);
    round27->setState(Round::Finished);
    date.addSecs(180);
    number++;
    round27->setNumber(number);
    round1->setStartTime(date);
    round27->setRe1Player(uli);
    round27->setRe2Player(niclas);
    round27->setContra1Player(patrick);
    round27->setContra2Player(niklas);
    Qp::update(round27);
    reGamePoints = 193;
    round27->setReGamePoints(reGamePoints);
    round27->setContraGamePoints(240-reGamePoints);
    rePoints = 6;
    round27->setPoints(round27->re1Player(), rePoints);
    round27->setPoints(round27->re2Player(), rePoints);
    round27->setPoints(round27->contra1Player(), -rePoints);
    round27->setPoints(round27->contra2Player(), -rePoints);
    round27->setTrumpfabgabePlayer(uli);
    round27->setTrumpfCount(1);
    round27->setTrumpfZurueck(0);
    round27->setSchweinereiPlayer(niclas);
    Qp::update(round27);

    QSharedPointer<Round> round28 = Qp::create<Round>();
    game->addRound(round28);
    round28->setState(Round::Finished);
    number++;
    round28->setNumber(number);
    date.addSecs(180);
    round28->setStartTime(date);
    round28->setRe1Player(mirko);
    round28->setRe2Player(johannes);
    round28->setContra1Player(stephan);
    round28->setContra2Player(timon);
    Qp::update(round28);
    reGamePoints = 83;
    round28->setReGamePoints(reGamePoints);
    round28->setContraGamePoints(240-reGamePoints);
    rePoints = -6;
    round28->setPoints(round28->re1Player(), rePoints);
    round28->setPoints(round28->re2Player(), rePoints);
    round28->setPoints(round28->contra1Player(), -rePoints);
    round28->setPoints(round28->contra2Player(), -rePoints);
    round28->setSchweinereiPlayer(stephan);
    Qp::update(round28);

    QSharedPointer<Round> round29 = Qp::create<Round>();
    game->addRound(round29);
    round29->setState(Round::Finished);
    number++;
    round29->setNumber(number);
    date.addSecs(180);
    round29->setStartTime(date);
    round29->setRe1Player(uli);
    round29->setRe2Player(timon);
    round29->setContra1Player(niclas);
    round29->setContra2Player(mirko);
    Qp::update(round29);
    reGamePoints = 160;
    round29->setReGamePoints(reGamePoints);
    round29->setContraGamePoints(240-reGamePoints);
    rePoints = -4;
    round29->setPoints(round29->re1Player(), rePoints);
    round29->setPoints(round29->re2Player(), rePoints);
    round29->setPoints(round29->contra1Player(), -rePoints);
    round29->setPoints(round29->contra2Player(), -rePoints);
    Qp::update(round29);

    QSharedPointer<Schmeisserei> schmeisserei2 = Qp::create<Schmeisserei>();
    schmeisserei2->setPlayer(mirko);
    schmeisserei2->setType(Schmeisserei::MoreThan4Kings);
    schmeisserei2->setKingsCount(5);
    round29->addSchmeisserei(schmeisserei2);
    Qp::update(schmeisserei2);
    Qp::update(round29);

    QSharedPointer<Round> round30 = Qp::create<Round>();
    game->addRound(round30);
    round30->setState(Round::Finished);
    number++;
    round30->setNumber(number);
    date.addSecs(180);
    round30->setStartTime(date);
    round30->setRe1Player(patrick);
    round30->setRe2Player(stephan);
    round30->setContra1Player(johannes);
    round30->setContra2Player(niklas);
    Qp::update(round30);
    reGamePoints = 187;
    round30->setReGamePoints(reGamePoints);
    round30->setContraGamePoints(240-reGamePoints);
    rePoints = 7;
    round30->setPoints(round30->re1Player(), rePoints);
    round30->setPoints(round30->re2Player(), rePoints);
    round30->setPoints(round30->contra1Player(), -rePoints);
    round30->setPoints(round30->contra2Player(), -rePoints);
    Qp::update(round30);

    QSharedPointer<Round> round31 = Qp::create<Round>();
    game->addRound(round31);
    round31->setState(Round::Finished);
    number++;
    round31->setNumber(number);
    date.addSecs(180);
    round31->setStartTime(date);
    round31->setRe1Player(timon);
    round31->setRe2Player(patrick);
    round31->setContra1Player(stephan);
    round31->setContra2Player(niklas);
    Qp::update(round31);
    reGamePoints = 156;
    round31->setReGamePoints(reGamePoints);
    round31->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round31->setPoints(round31->re1Player(), rePoints);
    round31->setPoints(round31->re2Player(), rePoints);
    round31->setPoints(round31->contra1Player(), -rePoints);
    round31->setPoints(round31->contra2Player(), -rePoints);
    Qp::update(round31);

    // SOLO
    QSharedPointer<Round> round32 = Qp::create<Round>();
    game->addRound(round32);
    round32->setState(Round::Finished);
    number++;
    round32->setNumber(number);
    date.addSecs(180);
    round32->setStartTime(date);
    round32->setRe1Player(niclas);
    round32->setContra1Player(uli);
    round32->setContra2Player(johannes);
    round32->setContra3Player(mirko);
    Qp::update(round32);
    reGamePoints = 160;
    round32->setReGamePoints(reGamePoints);
    round32->setContraGamePoints(240-reGamePoints);
    rePoints = 6;
    round32->setPoints(round32->re1Player(), rePoints*3);
    round32->setPoints(round32->contra1Player(), -rePoints);
    round32->setPoints(round32->contra2Player(), -rePoints);
    round32->setPoints(round32->contra3Player(), -rePoints);
    round32->setSoloPlayer(niclas);
    round32->setIsPflicht(true);
    round32->setSoloType(Round::BubenSolo);
    Qp::update(round32);

    // SOLO
    QSharedPointer<Round> round33 = Qp::create<Round>();
    game->addRound(round33);
    round33->setState(Round::Finished);
    number++;
    round33->setNumber(number);
    date.addSecs(180);
    round33->setStartTime(date);
    round33->setRe1Player(timon);
    round33->setContra1Player(patrick);
    round33->setContra2Player(stephan);
    round33->setContra3Player(johannes);
    Qp::update(round33);
    reGamePoints = 203;
    round33->setReGamePoints(reGamePoints);
    round33->setContraGamePoints(240-reGamePoints);
    rePoints = 3;
    round33->setPoints(round33->re1Player(), rePoints*3);
    round33->setPoints(round33->contra1Player(), -rePoints);
    round33->setPoints(round33->contra2Player(), -rePoints);
    round33->setPoints(round33->contra3Player(), -rePoints);
    round33->setSoloPlayer(timon);
    round33->setIsPflicht(true);
    round33->setSoloType(Round::BubenSolo);
    Qp::update(round33);

    QSharedPointer<Round> round34 = Qp::create<Round>();
    game->addRound(round34);
    round34->setState(Round::Finished);
    number++;
    round34->setNumber(number);
    date.addSecs(180);
    round34->setStartTime(date);
    round34->setRe1Player(niklas);
    round34->setRe2Player(mirko);
    round34->setContra1Player(niclas);
    round34->setContra2Player(uli);
    Qp::update(round34);
    reGamePoints = 171;
    round34->setReGamePoints(reGamePoints);
    round34->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round34->setPoints(round34->re1Player(), rePoints);
    round34->setPoints(round34->re2Player(), rePoints);
    round34->setPoints(round34->contra1Player(), -rePoints);
    round34->setPoints(round34->contra2Player(), -rePoints);
    Qp::update(round34);

    QSharedPointer<Schmeisserei> schmeisserei3 = Qp::create<Schmeisserei>();
    schmeisserei3->setPlayer(niklas);
    schmeisserei3->setType(Schmeisserei::MoreThan4Kings);
    schmeisserei3->setKingsCount(5);
    round34->addSchmeisserei(schmeisserei3);
    Qp::update(schmeisserei3);
    Qp::update(round34);

    // SOLO
    QSharedPointer<Round> round35 = Qp::create<Round>();
    game->addRound(round35);
    round35->setState(Round::Finished);
    number++;
    round35->setNumber(number);
    date.addSecs(180);
    round35->setStartTime(date);
    round35->setRe1Player(mirko);
    round35->setContra1Player(patrick);
    round35->setContra2Player(niklas);
    round35->setContra3Player(johannes);
    Qp::update(round35);
    reGamePoints = 183;
    round35->setReGamePoints(reGamePoints);
    round35->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round35->setPoints(round35->re1Player(), rePoints*3);
    round35->setPoints(round35->contra1Player(), -rePoints);
    round35->setPoints(round35->contra2Player(), -rePoints);
    round35->setPoints(round35->contra3Player(), -rePoints);
    round35->setSoloPlayer(mirko);
    round35->setIsPflicht(false);
    round35->setSoloType(Round::StilleHochzeit);
    Qp::update(round35);

    QSharedPointer<Round> round36 = Qp::create<Round>();
    game->addRound(round36);
    round36->setState(Round::Finished);
    number++;
    round36->setNumber(number);
    date.addSecs(180);
    round36->setStartTime(date);
    round36->setRe1Player(niclas);
    round36->setRe2Player(timon);
    round36->setContra1Player(uli);
    round36->setContra2Player(stephan);
    Qp::update(round36);
    reGamePoints = 127;
    round36->setReGamePoints(reGamePoints);
    round36->setContraGamePoints(240-reGamePoints);
    rePoints = 3;
    round36->setPoints(round36->re1Player(), rePoints);
    round36->setPoints(round36->re2Player(), rePoints);
    round36->setPoints(round36->contra1Player(), -rePoints);
    round36->setPoints(round36->contra2Player(), -rePoints);
    Qp::update(round36);

    QSharedPointer<Round> round37 = Qp::create<Round>();
    game->addRound(round37);
    round37->setState(Round::Finished);
    number++;
    round37->setNumber(number);
    date.addSecs(180);
    round37->setStartTime(date);
    round37->setRe1Player(mirko);
    round37->setRe2Player(stephan);
    round37->setContra1Player(niclas);
    round37->setContra2Player(niklas);
    Qp::update(round37);
    reGamePoints = 161;
    round37->setReGamePoints(reGamePoints);
    round37->setContraGamePoints(240-reGamePoints);
    rePoints = 4;
    round37->setPoints(round37->re1Player(), rePoints);
    round37->setPoints(round37->re2Player(), rePoints);
    round37->setPoints(round37->contra1Player(), -rePoints);
    round37->setPoints(round37->contra2Player(), -rePoints);
    round37->setSchweinereiPlayer(mirko);
    Qp::update(round37);

    QSharedPointer<Round> round38 = Qp::create<Round>();
    game->addRound(round38);
    round38->setState(Round::Finished);
    number++;
    round38->setNumber(number);
    date.addSecs(180);
    round38->setStartTime(date);
    round38->setRe1Player(timon);
    round38->setRe2Player(uli);
    round38->setContra1Player(patrick);
    round38->setContra2Player(johannes);
    Qp::update(round38);
    reGamePoints = 180;
    round38->setReGamePoints(reGamePoints);
    round38->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round38->setPoints(round38->re1Player(), rePoints);
    round38->setPoints(round38->re2Player(), rePoints);
    round38->setPoints(round38->contra1Player(), -rePoints);
    round38->setPoints(round38->contra2Player(), -rePoints);
    round38->setSchweinereiPlayer(timon);
    Qp::update(round38);

    QSharedPointer<Round> round39 = Qp::create<Round>();
    game->addRound(round39);
    round39->setState(Round::Finished);
    number++;
    round39->setNumber(number);
    date.addSecs(180);
    round39->setStartTime(date);
    round39->setRe1Player(niclas);
    round39->setRe2Player(stephan);
    round39->setContra1Player(uli);
    round39->setContra2Player(johannes);
    Qp::update(round39);
    reGamePoints = 196;
    round39->setReGamePoints(reGamePoints);
    round39->setContraGamePoints(240-reGamePoints);
    rePoints = 8;
    round39->setPoints(round39->re1Player(), rePoints);
    round39->setPoints(round39->re2Player(), rePoints);
    round39->setPoints(round39->contra1Player(), -rePoints);
    round39->setPoints(round39->contra2Player(), -rePoints);
    round39->setHochzeitPlayer(niclas);
    round39->setHochzeitDecision(Round::ErsterFehl);
    round39->setSchweinereiPlayer(stephan);
    Qp::update(round39);

    QSharedPointer<Round> round40 = Qp::create<Round>();
    game->addRound(round40);
    round40->setState(Round::Finished);
    date.addSecs(180);
    number++;
    round40->setNumber(number);
    round1->setStartTime(date);
    round40->setRe1Player(mirko);
    round40->setRe2Player(niklas);
    round40->setContra1Player(patrick);
    round40->setContra2Player(timon);
    Qp::update(round40);
    reGamePoints = 102;
    round40->setReGamePoints(reGamePoints);
    round40->setContraGamePoints(240-reGamePoints);
    rePoints = -4;
    round40->setPoints(round40->re1Player(), rePoints);
    round40->setPoints(round40->re2Player(), rePoints);
    round40->setPoints(round40->contra1Player(), -rePoints);
    round40->setPoints(round40->contra2Player(), -rePoints);
    round40->setTrumpfabgabePlayer(mirko);
    round40->setTrumpfCount(3);
    round40->setTrumpfZurueck(2);
    Qp::update(round40);

    QSharedPointer<Round> round41 = Qp::create<Round>();
    game->addRound(round41);
    round41->setState(Round::Finished);
    number++;
    round41->setNumber(number);
    date.addSecs(180);
    round41->setStartTime(date);
    round41->setRe1Player(niklas);
    round41->setRe2Player(johannes);
    round41->setContra1Player(uli);
    round41->setContra2Player(timon);
    Qp::update(round41);
    reGamePoints = 184;
    round41->setReGamePoints(reGamePoints);
    round41->setContraGamePoints(240-reGamePoints);
    rePoints = 8;
    round41->setPoints(round41->re1Player(), rePoints);
    round41->setPoints(round41->re2Player(), rePoints);
    round41->setPoints(round41->contra1Player(), -rePoints);
    round41->setPoints(round41->contra2Player(), -rePoints);
    Qp::update(round41);

    QSharedPointer<Schmeisserei> schmeisserei4 = Qp::create<Schmeisserei>();
    schmeisserei4->setPlayer(johannes);
    schmeisserei4->setType(Schmeisserei::MoreThan4Kings);
    schmeisserei4->setKingsCount(5);
    round41->addSchmeisserei(schmeisserei4);
    Qp::update(schmeisserei4);
    Qp::update(round41);

    QSharedPointer<Round> round42 = Qp::create<Round>();
    game->addRound(round42);
    round42->setState(Round::Finished);
    date.addSecs(180);
    number++;
    round42->setNumber(number);
    round1->setStartTime(date);
    round42->setRe1Player(niclas);
    round42->setRe2Player(mirko);
    round42->setContra1Player(stephan);
    round42->setContra2Player(patrick);
    Qp::update(round42);
    reGamePoints = 141;
    round42->setReGamePoints(reGamePoints);
    round42->setContraGamePoints(240-reGamePoints);
    rePoints = 3;
    round42->setPoints(round42->re1Player(), rePoints);
    round42->setPoints(round42->re2Player(), rePoints);
    round42->setPoints(round42->contra1Player(), -rePoints);
    round42->setPoints(round42->contra2Player(), -rePoints);
    round42->setTrumpfabgabePlayer(niclas);
    round42->setTrumpfCount(3);
    round42->setTrumpfZurueck(2);
    round42->setSchweinereiPlayer(stephan);
    Qp::update(round42);

    QSharedPointer<Round> round43 = Qp::create<Round>();
    game->addRound(round43);
    round43->setState(Round::Finished);
    number++;
    round43->setNumber(number);
    date.addSecs(180);
    round43->setStartTime(date);
    round43->setRe1Player(niclas);
    round43->setRe2Player(timon);
    round43->setContra1Player(niklas);
    round43->setContra2Player(patrick);
    Qp::update(round43);
    reGamePoints = 209;
    round43->setReGamePoints(reGamePoints);
    round43->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round43->setPoints(round43->re1Player(), rePoints);
    round43->setPoints(round43->re2Player(), rePoints);
    round43->setPoints(round43->contra1Player(), -rePoints);
    round43->setPoints(round43->contra2Player(), -rePoints);
    Qp::update(round43);

    QSharedPointer<Schmeisserei> schmeisserei5 = Qp::create<Schmeisserei>();
    schmeisserei5->setPlayer(patrick);
    schmeisserei5->setType(Schmeisserei::MoreThan4Kings);
    schmeisserei5->setKingsCount(5);
    round43->addSchmeisserei(schmeisserei5);
    Qp::update(schmeisserei5);
    Qp::update(round43);

    QSharedPointer<Round> round44 = Qp::create<Round>();
    game->addRound(round44);
    round44->setState(Round::Finished);
    number++;
    round44->setNumber(number);
    date.addSecs(180);
    round44->setStartTime(date);
    round44->setRe1Player(johannes);
    round44->setRe2Player(uli);
    round44->setContra1Player(mirko);
    round44->setContra2Player(stephan);
    Qp::update(round44);
    reGamePoints = 176;
    round44->setReGamePoints(reGamePoints);
    round44->setContraGamePoints(240-reGamePoints);
    rePoints = 5;
    round44->setPoints(round44->re1Player(), rePoints);
    round44->setPoints(round44->re2Player(), rePoints);
    round44->setPoints(round44->contra1Player(), -rePoints);
    round44->setPoints(round44->contra2Player(), -rePoints);
    round44->setSchweinereiPlayer(johannes);
    Qp::update(round44);

    QSharedPointer<Round> round45 = Qp::create<Round>();
    game->addRound(round45);
    round45->setState(Round::Finished);
    number++;
    round45->setNumber(number);
    date.addSecs(180);
    round45->setStartTime(date);
    round45->setRe1Player(niclas);
    round45->setRe2Player(mirko);
    round45->setContra1Player(uli);
    round45->setContra2Player(patrick);
    Qp::update(round45);
    reGamePoints = 141;
    round45->setReGamePoints(reGamePoints);
    round45->setContraGamePoints(240-reGamePoints);
    rePoints = 3;
    round45->setPoints(round45->re1Player(), rePoints);
    round45->setPoints(round45->re2Player(), rePoints);
    round45->setPoints(round45->contra1Player(), -rePoints);
    round45->setPoints(round45->contra2Player(), -rePoints);
    round45->setSchweinereiPlayer(niclas);
    Qp::update(round45);

    QSharedPointer<Round> round46 = Qp::create<Round>();
    game->addRound(round46);
    round46->setState(Round::Finished);
    number++;
    round46->setNumber(number);
    date.addSecs(180);
    round46->setStartTime(date);
    round46->setRe1Player(niklas);
    round46->setRe2Player(stephan);
    round46->setContra1Player(johannes);
    round46->setContra2Player(timon);
    Qp::update(round46);
    reGamePoints = 192;
    round46->setReGamePoints(reGamePoints);
    round46->setContraGamePoints(240-reGamePoints);
    rePoints = 9;
    round46->setPoints(round46->re1Player(), rePoints);
    round46->setPoints(round46->re2Player(), rePoints);
    round46->setPoints(round46->contra1Player(), -rePoints);
    round46->setPoints(round46->contra2Player(), -rePoints);
    Qp::update(round46);

    // SOLO
    QSharedPointer<Round> round47 = Qp::create<Round>();
    game->addRound(round47);
    round47->setState(Round::Finished);
    number++;
    round47->setNumber(number);
    date.addSecs(180);
    round47->setStartTime(date);
    round47->setRe1Player(uli);
    round47->setContra1Player(timon);
    round47->setContra2Player(mirko);
    round47->setContra3Player(stephan);
    Qp::update(round47);
    reGamePoints = 89;
    round47->setReGamePoints(reGamePoints);
    round47->setContraGamePoints(240-reGamePoints);
    rePoints = -4;
    round47->setPoints(round47->re1Player(), rePoints*3);
    round47->setPoints(round47->contra1Player(), -rePoints);
    round47->setPoints(round47->contra2Player(), -rePoints);
    round47->setPoints(round47->contra3Player(), -rePoints);
    round47->setSoloPlayer(uli);
    round47->setIsPflicht(true);
    round47->setSoloType(Round::DamenSolo);
    Qp::update(round47);

    // SOLO
    QSharedPointer<Round> round48 = Qp::create<Round>();
    game->addRound(round48);
    round48->setState(Round::Finished);
    number++;
    round48->setNumber(number);
    date.addSecs(180);
    round48->setStartTime(date);
    round48->setRe1Player(patrick);
    round48->setContra1Player(niclas);
    round48->setContra2Player(johannes);
    round48->setContra3Player(niklas);
    Qp::update(round48);
    reGamePoints = 66;
    round48->setReGamePoints(reGamePoints);
    round48->setContraGamePoints(240-reGamePoints);
    rePoints = -2;
    round48->setPoints(round48->re1Player(), rePoints*3);
    round48->setPoints(round48->contra1Player(), -rePoints);
    round48->setPoints(round48->contra2Player(), -rePoints);
    round48->setPoints(round48->contra3Player(), -rePoints);
    round48->setSoloPlayer(patrick);
    round48->setIsPflicht(true);
    round48->setSoloType(Round::TrumpfSolo);
    Qp::update(round48);

    Qp::update(game);

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
