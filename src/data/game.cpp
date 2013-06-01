#include "game.h"

#include "place.h"
#include "player.h"
#include "round.h"
#include "livedrink.h"

#include <QTimer>



Game::Game(QObject *parent) :
    QObject(parent),
    m_type(UnkownType),
    m_creationTime(QDateTime::currentDateTime()),
    m_mitPflichtSolo(false),
    m_site("site", this),
    m_players("players", this),
    m_rounds("rounds", this)
{
    m_lengthTimer.setInterval(1000);
    m_lengthTimer.setSingleShot(false);

    QObject::connect(&m_lengthTimer, &QTimer::timeout, [&] {
        QSharedPointer<Round> r = currentRound();
        if(r)
            r->setLength(r->length().addSecs(1));
    });
}

Game::~Game()
{
}

QTime Game::length() const
{
    QTime time;
    foreach(QSharedPointer<Round> r, rounds()) {
        QTime t = r->length();
        time.addSecs(t.hour() * 60 * 60
                     + t.minute() * 60
                     + t.second());
    }
    return time;
}

Game::State Game::state() const
{
    QSharedPointer<Round> r = currentRound();
    if(!r)
        return UnkownState;

    switch(r->state()) {
    case Round::Finished:
        return Finished;
    case Round::Running:
        return Running;
    case Round::UnkownState:
    default:
        return UnkownState;
    }

    return UnkownState;
}

void Game::setState(State state)
{
    if(state == Running)
        m_lengthTimer.start();
    else
        m_lengthTimer.stop();

    QSharedPointer<Round> r = currentRound();
    if(!r)
        return;

    switch(state) {
    case Finished:
        r->setState(Round::Finished);
        return;
    case Running:
        r->setState(Round::Running);
        return;
    case UnkownState:
    default:
        r->setState(Round::UnkownState);
        return;
    }
}

QString Game::comment() const
{
    return m_comment;
}

void Game::setComment(const QString &comment)
{
    m_comment = comment;
}

QDateTime Game::creationTime() const
{
    return m_creationTime;
}

void Game::setCreationTime(const QDateTime &startingDate)
{
    m_creationTime = startingDate;
}

Game::Type Game::type() const
{
    return m_type;
}

void Game::setType(const Type &type)
{
    m_type = type;
}

int Game::_type() const
{
    return static_cast<int>(type());
}

void Game::_setType(int type)
{
    setType(static_cast<Type>(type));
}

QString Game::name() const
{
    return m_name;
}

void Game::setName(const QString &name)
{
    m_name = name;
}

QSharedPointer<Place> Game::site() const
{
    return m_site.resolve();
}

void Game::setSite(QSharedPointer<Place> site)
{
    m_site.relate(site);
}

QList<QSharedPointer<Player> > Game::players() const
{
    return m_players.resolveList();
}

QSharedPointer<Player> Game::currentCardMixer() const
{
    if(isComplete())
        return QSharedPointer<Player>();

    QSharedPointer<Round> round = currentRound();
    if(!round)
        return QSharedPointer<Player>();

    return round->cardMixer();
}

QList<QSharedPointer<Player> > Game::currentPlayingPlayers() const
{
    if(isComplete())
        return QList<QSharedPointer<Player> >();

    QSharedPointer<Round> round = currentRound();
    if(!round)
        return QList<QSharedPointer<Player> >();

    return round->playingPlayers();
}

void Game::setPlayers(const QList<QSharedPointer<Player> > &players)
{
    m_players.clear();
    m_players.relate(players);
}

QList<QSharedPointer<Round> > Game::rounds() const
{
    return m_rounds.resolveList();
}

QSharedPointer<Round> Game::currentRound() const
{
    QList<QSharedPointer<Round> > rs = rounds();
    if(rs.isEmpty())
        return QSharedPointer<Round>();

    return rs.last();
}

QList<QSharedPointer<Player> > Game::playersSortedByPlacement() const
{
    QSharedPointer<Round> round = currentRound();
    if(!round)
        return QList<QSharedPointer<Player> >();

    return round->playersSortedByPlacement();
}

int Game::totalPoints() const
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        result += qAbs(round->points());
    }
    return result;
}

int Game::totalPoints(QSharedPointer<Player> player) const
{
    QSharedPointer<Round> round = currentRound();
    if(!round)
        return 0;

    return round->totalPoints(player);
}

QList<QSharedPointer<LiveDrink> > Game::drinks() const
{
    QList<QSharedPointer<LiveDrink> > result;
    foreach(QSharedPointer<Round> round, rounds()) {
        result.append(round->drinks());
    }
    return result;
}

QList<QSharedPointer<LiveDrink> > Game::drinks(QSharedPointer<Player> player) const
{
    QList<QSharedPointer<LiveDrink> > result;
    foreach(QSharedPointer<Round> round, rounds()) {
        foreach(QSharedPointer<LiveDrink> drink, round->drinks()) {
            if(drink->player() == player)
                result.append(drink);
        }
    }
    return result;
}

double Game::completedPercentage() const
{
    int totalRoundCount = players().size() * 5;
    if(totalRoundCount == 0)
        return 0;

    int finishedRoundCount = rounds().size();

    // If the game is not finished, the last round is not finished.
    if(state() != Finished)
        --finishedRoundCount;

    return finishedRoundCount / totalRoundCount;
}

bool Game::isComplete() const
{
    int totalRoundCount = players().size() * 5;
    int finishedRoundCount = rounds().size();

    // If the game is not finished, the last round is not finished.
    if(state() != Finished)
        --finishedRoundCount;

    return totalRoundCount == finishedRoundCount;
}

bool Game::hasPflichtSolo(QSharedPointer<Player> player) const
{
    foreach(QSharedPointer<Round> round, rounds()) {
        if(round->isPflicht() && round->soloPlayer() == player) {
            return true;
        }
    }
    return false;
}

int Game::hochzeitCountAfterRounds(int roundCount)
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(roundCount == 0)
            break;

        --roundCount;
        if(round->hochzeitPlayer()) {
            ++result;
        }
    }
    return result;
}

int Game::soloCountAfterRounds(int roundCount)
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(roundCount == 0)
            break;

        --roundCount;
        if(round->soloPlayer()) {
            ++result;
        }
    }
    return result;
}

int Game::pflichtSoloCountAfterRounds(int roundCount)
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(roundCount == 0)
            break;

        --roundCount;
        if(round->isPflicht() && round->hochzeitPlayer()) {
            ++result;
        }
    }
    return result;
}

int Game::trumpfabgabeCountAfterRounds(int roundCount)
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(roundCount == 0)
            break;

        --roundCount;
        if(round->trumpfabgabePlayer()) {
            ++result;
        }
    }
    return result;
}

int Game::schweinereiCountAfterRounds(int roundCount)
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(roundCount == 0)
            break;

        --roundCount;
        if(round->trumpfabgabePlayer()) {
            ++result;
        }
    }
    return result;
}

int Game::schmeissereiCountAfterRounds(int roundCount)
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(roundCount == 0)
            break;

        --roundCount;
        result += round->schmeissereien().size();
    }
    return result;
}

int Game::placement(QSharedPointer<Player> player) const
{
    QSharedPointer<Round> round = currentRound();
    if(!round)
        return -1;

    return round->placement(player);
}

int Game::placementAfterRounds(QSharedPointer<Player> player, int roundNumber) const
{
    QList<QSharedPointer<Round> > rs = rounds();
    if(roundNumber - 1 <= rs.size())
        return -1;

    return rs.at(roundNumber - 1)->placement(player);
}

double Game::averagePlacement(QSharedPointer<Player> player) const
{
    double result = 0.0;
    int roundsPlayedCount = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(round->playingPlayers().contains(player)) {
            ++roundsPlayedCount;
            result += round->placement(player);
        }
    }

    if(roundsPlayedCount == 0)
        return 0.0;

    return result / roundsPlayedCount;
}

int Game::leadingRoundCount(QSharedPointer<Player> player) const
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        QList<QSharedPointer<Player> > ps = round->playersSortedByPlacement();
        if(ps.isEmpty())
            continue;

        if(ps.first() == player)
            ++result;
    }
    return result;
}

void Game::setRounds(const QList<QSharedPointer<Round> > &rounds)
{
    m_rounds.clear();
    m_rounds.relate(rounds);
}

void Game::setMitPflichtSolo(bool arg)
{
    m_mitPflichtSolo = arg;
}

bool Game::mitPflichtSolo() const
{
    return m_mitPflichtSolo;
}
