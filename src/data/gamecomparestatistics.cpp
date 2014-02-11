#include "gamecomparestatistics.h"

#include "game.h"
#include "round.h"
#include <QPersistence.h>

#include <qDebug>
#include <QElapsedTimer>

GameCompareStatistics::GameCompareStatistics(QObject *parent) :
    QObject(parent),
    m_initialized(false),
    m_useAllGames(true)
{
    connect(Qp::dataAccessObject<Round>(), SIGNAL(objectUpdated(QSharedPointer<QObject>)), this, SLOT(updateRound(QSharedPointer<QObject>)));
}

GameCompareStatistics &GameCompareStatistics::instance()
{
    static GameCompareStatistics inst;
    return inst;
}

int GameCompareStatistics::placementOfGame(QSharedPointer<Game> game, Slot::Context context, int roundNumber)
{
    if(!m_initialized) {
        initialize();
    }

    return m_slots.value(roundNumber)->placement(game, context);
}

int GameCompareStatistics::value(QSharedPointer<Game> game, Slot::Context context, int roundNumber)
{
    if(!m_initialized) {
        initialize();
    }

    return m_slots.value(roundNumber)->value(game->rounds().at(roundNumber), context);
}

void GameCompareStatistics::setGames(QList<QSharedPointer<Game> > games)
{
    reset();

    m_games = games;
    m_useAllGames = false;
}

QList<QSharedPointer<Game> > GameCompareStatistics::games()
{
    if(m_useAllGames) {
        return Qp::readAll<Game>();
    }
    else {
        return m_games;
    }
}

void GameCompareStatistics::reset()
{
    m_slots.clear();
    m_games.clear();

    m_initialized = false;
    m_useAllGames = true;
}

void GameCompareStatistics::updateRound(QSharedPointer<QObject> object)
{
    QSharedPointer<Round> round = qSharedPointerCast<Round>(object);

}

void GameCompareStatistics::initialize()
{
    QElapsedTimer timer;
    timer.start();


    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            if(!m_slots.keys().contains(round->number())) {
                Slot* slot = new Slot(this);
                m_slots.insert(round->number(), slot);
            }
        }
    }

    foreach(int number, m_slots.keys()) {
        if(m_slots.keys().contains(number-1)) {
            m_slots.value(number)->setPreviousSlot(m_slots.value(number-1));
        }
    }

    foreach(QSharedPointer<Game> game, games()) {
        foreach(QSharedPointer<Round> round, game->rounds()) {
            m_slots.value(round->number())->addRound(round);
        }
    }

    qDebug() << "Initialize of GameCompareStatistics took" << timer.elapsed() << "milliseconds";
    qDebug() << "Count of games: " << games().size();
    qDebug() << "Count of slots: " << m_slots.keys().size();

    m_initialized = true;
}


Slot::Slot(QObject *parent):
    QObject(parent),
    m_prevSlot(0)
{}

Slot::~Slot()
{
    if(m_prevSlot) {
        m_prevSlot = 0;
        delete m_prevSlot;
    }
}

QMap<Slot::Context, QList<GamePair> > Slot::data()
{
    return m_data;
}

int Slot::placement(QSharedPointer<Game> game, Slot::Context context)
{
    QList<GamePair> pairs = m_data.value(context);

    int value = -1;
    foreach(GamePair pair, pairs) {
        if(game == pair.first) {
            value = pair.second;
        }
    }

    int placement = 1;

    foreach(GamePair pair, pairs) {
        if(pair.second > value) {
            placement++;
        }
    }

    return placement;
}

void Slot::addRound(QSharedPointer<Round> round)
{
    if(round->hochzeitPlayer()) {
        addValue(round, Hochzeit, 1);
    }
    else {
        addValue(round, Hochzeit, 0);
    }
    if(round->soloPlayer()) {
        addValue(round, Solo, 1);
    }
    else {
        addValue(round, Solo, 0);
    }
    if(round->trumpfabgabePlayer()) {
        addValue(round, Trumpfabgabe, 1);
    }
    else {
        addValue(round, Trumpfabgabe, 0);
    }
    if(round->schweinereiPlayer()) {
        addValue(round, Schweinerei, 1);
    }
    else {
        addValue(round, Schweinerei, 0);
    }
    if(round->schmeissereien().size() > 0) {
        addValue(round, Schmeisserei, round->schmeissereien().size());
    }
    else {
        addValue(round, Schmeisserei, 0);
    }
}

void Slot::setPreviousSlot(Slot *slot)
{
    m_prevSlot = slot;
}

void Slot::addValue(QSharedPointer<Round> round, Slot::Context context, int value)
{
    QList<QPair<QSharedPointer<Game>, int> > list;
    if(m_data.contains(context)) {
        list.append(m_data.value(context));
    }

    int prevValue = 0;
    if(m_prevSlot) {
        prevValue = m_prevSlot->value(round, context);
    }

    QPair<QSharedPointer<Game>, int> pair;
    pair.first = round->game();
    pair.second = value + prevValue;

    list.append(pair);
    m_data.insert(context, list);
}

int Slot::value(QSharedPointer<Round> round, Context context)
{
    return pair(context, round).second;
}

GamePair Slot::pair(Context context, QSharedPointer<Round> round)
{
    foreach(GamePair pair, m_data.value(context)) {
        if(pair.first == round->game()) {
            return pair;
        }
    }

    Q_ASSERT_X(false,Q_FUNC_INFO,"Corresponding pair not found!");

    return GamePair();
}
