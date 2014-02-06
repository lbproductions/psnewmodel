#include "game.h"

#include "place.h"
#include "player.h"
#include "round.h"
#include "livedrink.h"
#include "league.h"
#include "drink.h"
#include "old_offlineGameInformation.h"
#include "old_dokoofflinegamebuddys.h"

#include <QTimer>


Game::Game(QObject *parent) :
    QObject(parent),
    m_type(UnkownType),
    m_creationTime(QDateTime::currentDateTime()),
    m_mitPflichtSolo(false),
    m_site("site", this),
    m_players("players", this),
    m_rounds("rounds", this),
    m_leagues("leagues",this),
    m_offlineGameInformation("offlineGameInformation", this),
    m_dokoOfflineGameBuddys("dokoOfflineGameBuddys",this)
{
    m_lengthTimer.setInterval(1000);
    m_lengthTimer.setSingleShot(false);

    QObject::connect(&m_lengthTimer, &QTimer::timeout, [&] {
        QSharedPointer<Round> r = currentRound();
        if(r) {
            QTime length = r->length();
            if(!length.isValid())
                length = QTime(0,0,0);
            r->setLength(length.addSecs(1));
            emit lengthChanged();
        }
    });
}

Game::~Game()
{
}

void Game::save()
{
    Qp::update(Qp::sharedFrom(this));
    Qp::update(currentRound());
}

QTime Game::length() const
{
    QTime time(0,0,0);
    foreach(QSharedPointer<Round> r, rounds()) {
        QTime t = r->length();
        int secs = t.hour() * 60 * 60
                + t.minute() * 60
                + t.second();
        time = time.addSecs(secs);
    }
    return time;
}

QTime Game::finishedRoundsLength() const
{
    QTime time(0,0,0);
    foreach(QSharedPointer<Round> r, rounds()) {
        if(r->state() == Round::Finished) {
            QTime t = r->length();
            time = time.addSecs(QTime(0,0,0).secsTo(t));
        }
    }
    return time;
}

QTime Game::averageRoundLength(double weight) const
{
    QList<QSharedPointer<Round> > rs = rounds();

    if(rs.size() <= 1)
        return length();

    QSharedPointer<Round> current = rs.last();
    double weightedAverageSecs = 0;
    foreach(QSharedPointer<Round> r, rs) {
        if(r == current) // dont account for the running round
            break;

        QTime t = r->length();
        double secs = t.hour() * 60 * 60
                + t.minute() * 60
                + t.second();
        if(weightedAverageSecs == 0) {
            weightedAverageSecs = secs;
        }
        else {
            weightedAverageSecs = secs * weight + weightedAverageSecs * (1 - weight);
        }
    }

    return QTime(0,0,0).addSecs(weightedAverageSecs);
}

QTime Game::predictedTimeToPlay(double weight) const
{
    int averageLength = QTime(0,0,0).secsTo(averageRoundLength(weight));
    return QTime(0,0,0).addSecs(averageLength*roundsToPlay());
}

QTime Game::predictedEndTime(double weight) const
{
    int averageLength = QTime(0,0,0).secsTo(averageRoundLength(weight));
    return QTime::currentTime().addSecs(averageLength*roundsToPlay());
}

QTime Game::predictedGameLength(double weight) const
{
    return finishedRoundsLength().addSecs(QTime(0,0,0).secsTo(predictedTimeToPlay(weight)));
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
    case Round::Paused:
        return Paused;
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
        Qp::remove<Round>(r);
        emit stateChanged();
        return;
    case Running:
        r->setState(Round::Running);
        connectAllRoundSignals();
        emit stateChanged();
        return;
    case Paused:
        r->setState(Round::Paused);
        emit stateChanged();
        return;
    case UnkownState:
    default:
        r->setState(Round::UnkownState);
        return;
    }

    emit stateChanged();
}

void Game::togglePlayPause()
{
    State s = state();
    if(s == Game::Running) {
        setState(Game::Paused);
    }
    else if(s == Game::Paused) {
        setState(Game::Running);
    }
}

void Game::pause()
{
    if(state() == Game::Running)
        setState(Game::Paused);
}

QPixmap Game::statePixmap() const
{
    static const QPixmap RunningStatePixmap(":/general/state-running.png");
    static const QPixmap PausedStatePixmap(":/general/state-paused.png");
    static const QPixmap FinishedStatePixmap(":/general/state-finished.png");

    switch(state()) {
    case Finished:
        return FinishedStatePixmap;
    case Running:
        return RunningStatePixmap;
    case Paused:
        return PausedStatePixmap;
    case UnkownState:
    default:
        break;
    }

    return QPixmap();
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
    site->addGame(Qp::sharedFrom(this));
    m_site.relate(site);
}

QList<QSharedPointer<Player> > Game::players() const
{
    return m_players.resolveList();
}

void Game::addPlayer(QSharedPointer<Player> player)
{
    m_players.relate(player);
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
    QList<QSharedPointer<Round> > rs = m_rounds.resolveList();
    return rs;
}

QSharedPointer<Round> Game::currentRound() const
{
    if(m_currentRoundCached)
        return m_currentRoundCached;

    QList<QSharedPointer<Round> > rs = rounds();
    if(rs.isEmpty())
        return QSharedPointer<Round>();

    m_currentRoundCached = rs.last();
    return m_currentRoundCached;
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

int Game::roundsTogether(QSharedPointer<Player> playerOne, QSharedPointer<Player> playerTwo)
{
    int count = 0;

    foreach(QSharedPointer<Round> round, rounds()) {
        if(round->state() == Round::Finished) {
            if(round->playingPlayers().contains(playerOne) && round->playingPlayers().contains(playerTwo)) {
                if((round->rePlayers().contains(playerOne) && round->rePlayers().contains(playerTwo)) ||
                        (!round->rePlayers().contains(playerOne) && !round->rePlayers().contains(playerTwo))) {
                    count++;
                }
            }
        }
    }

    return count;
}

int Game::winsTogether(QSharedPointer<Player> playerOne, QSharedPointer<Player> playerTwo)
{
    int count = 0;

    foreach(QSharedPointer<Round> round, rounds()) {
        if(round->state() == Round::Finished) {
            if(round->playingPlayers().contains(playerOne) && round->playingPlayers().contains(playerTwo)) {
                if((round->rePlayers().contains(playerOne) && round->rePlayers().contains(playerTwo)) ||
                        (!round->rePlayers().contains(playerOne) && !round->rePlayers().contains(playerTwo))) {
                    if(round->points(playerOne) > 0) {
                        count++;
                    }
                }
            }
        }
    }

    return count;
}

int Game::pointsToLeader(QSharedPointer<Player> player)
{
    if(!currentRound()) {
        return 0;
    }

    else return currentRound()->pointsToLeader(player);
}

int Game::drinkCount(QSharedPointer<Drink> drink) const
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        foreach(QSharedPointer<LiveDrink> liveDrink, round->liveDrinks()) {
            if(drink == liveDrink->drink())
                ++result;
        }
    }
    return result;
}

QMap<QSharedPointer<Drink>, int> Game::drinkCounts(QSharedPointer<Player> player) const
{
    QMap<QSharedPointer<Drink>, int> result;
    foreach(QSharedPointer<Round> round, rounds()) {
        foreach(QSharedPointer<LiveDrink> drink, round->liveDrinks()) {
            if(drink->player() == player)
                ++result[drink->drink()];
        }
    }
    return result;
}

int Game::totalDrinkCount(QSharedPointer<Player> player) const
{
    int count = 0;

    QMap<QSharedPointer<Drink>, int> map = drinkCounts(player);
    foreach(QSharedPointer<Drink> drink, map.keys()) {
        count += map.value(drink);
    }

    return count;
}

QList<QSharedPointer<LiveDrink> > Game::liveDrinks() const
{
    QList<QSharedPointer<LiveDrink> > result;
    foreach(QSharedPointer<Round> round, rounds()) {
        result.append(round->liveDrinks());
    }
    return result;
}

QList<QSharedPointer<LiveDrink> > Game::liveDrinks(QSharedPointer<Player> player) const
{
    QList<QSharedPointer<LiveDrink> > result;
    foreach(QSharedPointer<Round> round, rounds()) {
        foreach(QSharedPointer<LiveDrink> drink, round->liveDrinks()) {
            if(drink->player() == player)
                result.append(drink);
        }
    }
    return result;
}

void Game::startNextRound()
{
    int nextNumber = 0;

    QSharedPointer<Round> round = currentRound();
    if(round) {
        nextNumber = round->number() + 1;
        round->setState(Round::Finished);
        Qp::update(round);
    }

    round = Qp::create<Round>();
    round->setGame(Qp::sharedFrom(this));
    round->setStartTime(QDateTime::currentDateTime());
    round->setNumber(nextNumber);
    round->setState(Round::Running);
    connectRoundSignals(round);
    addRound(round);
    save();

    emit newRoundStarted();
}

int Game::totalRoundCount() const
{
    return players().size() * 6;
}

int Game::finishedRoundCount() const
{
    int finishedRoundCount = rounds().size();

    // If the game is not finished, the last round is not finished.
    if(state() != Finished)
        --finishedRoundCount;

    return finishedRoundCount;
}

double Game::completedPercentage() const
{
    int total = totalRoundCount();
    if(total == 0)
        return 0;

    return (double) finishedRoundCount() * 100 / (double) total;
}

bool Game::isComplete() const
{
    return totalRoundCount() == finishedRoundCount();
}

int Game::roundsToPlay() const
{
    return totalRoundCount() - finishedRoundCount();
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

int Game::normalRoundCount(int roundCount)
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(roundCount == 0)
            break;

        --roundCount;
        if(!round->hochzeitPlayer() && !round->soloPlayer() && !round->trumpfabgabePlayer()) {
            ++result;
        }
    }
    return result;
}

int Game::hochzeitCount(int roundCount)
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

int Game::soloCount(int roundCount)
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

int Game::pflichtSoloCount(int roundCount)
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

int Game::trumpfabgabeCount(int roundCount)
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

int Game::schweinereiCount(int roundCount)
{
    int result = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(roundCount == 0)
            break;

        --roundCount;
        if(round->schweinereiPlayer()) {
            ++result;
        }
    }
    return result;
}

int Game::schmeissereiCount(int roundCount)
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

int Game::reWinsCount()
{
    int reCount = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(round->winnerParty() == Round::Re) {
            reCount++;
        }
    }

    return reCount;
}

int Game::contraWinCount()
{
    int contra = 0;
    foreach(QSharedPointer<Round> round, rounds()) {
        if(round->winnerParty() == Round::Contra) {
            contra++;
        }
    }

    return contra;
}

QList<QSharedPointer<OLD_OfflineGameInformation> > Game::offlineGameInformation() const
{
    return m_offlineGameInformation.resolveList();
}

QList<QSharedPointer<OLD_DokoOfflineGameBuddys> > Game::dokoOfflineGameBuddys() const
{
    return m_dokoOfflineGameBuddys.resolveList();
}

int Game::placement(QSharedPointer<Player> player, int roundNumber) const
{
    QList<QSharedPointer<Round> > rs = rounds();

    if(roundNumber >= rs.size())
        roundNumber = rs.size() - 1;

    return rs.at(roundNumber)->placement(player);
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
    m_currentRoundCached = QSharedPointer<Round>();
}

void Game::addRound(QSharedPointer<Round> round)
{
    m_rounds.relate(round);
    m_currentRoundCached = QSharedPointer<Round>();
}

void Game::setMitPflichtSolo(bool arg)
{
    m_mitPflichtSolo = arg;
}

bool Game::mitPflichtSolo() const
{
    return m_mitPflichtSolo;
}

bool sortGamesByDate(const QSharedPointer<Game> &g1, const QSharedPointer<Game> &g2)
{
    return g1->creationTime() < g2->creationTime();
}

QList<QSharedPointer<League> > Game::leagues() const
{
    return m_leagues.resolveList();
}

void Game::setLeagues(const QList<QSharedPointer<League> > &arg)
{
    m_leagues.relate(arg);
}

void Game::setOfflineGameInformation(const QList<QSharedPointer<OLD_OfflineGameInformation> > &games)
{
    m_offlineGameInformation.relate(games);
}

void Game::setDokoOfflineGameBuddys(const QList<QSharedPointer<OLD_DokoOfflineGameBuddys> > &games)
{
    m_dokoOfflineGameBuddys.relate(games);
}

void Game::connectRoundSignals(QSharedPointer<Round> round)
{
    connect(round.data(), &Round::schmeissereiAdded, this, &Game::schmeissereiAdded);
    connect(round.data(), &Round::drinkAdded, this, &Game::drinksChanged);
    connect(round.data(), &Round::drinkRemoved, this, &Game::drinksChanged);
}

void Game::connectAllRoundSignals()
{
    foreach(QSharedPointer<Round> round, rounds()) {
        connectRoundSignals(round);
    }
}
