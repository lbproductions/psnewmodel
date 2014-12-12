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
    m_additionalMissingPlayer(MissingOppositeOfCardMixer),
    m_site(QpRelation(&Game::site)),
    m_players(QpRelation(&Game::players)),
    m_rounds(QpRelation(&Game::rounds)),
    m_offlineGameInformation(QpRelation(&Game::offlineGameInformation)),
    m_initialCalcedStats(false)
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

    connect(this, SIGNAL(newRoundStarted()), this, SLOT(updateStats()));
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

QTime Game::predictedTimeToPlay(double weight)
{
    int averageLength = QTime(0,0,0).secsTo(averageRoundLength(weight));
    return QTime(0,0,0).addSecs(averageLength*roundsToPlay());
}

QTime Game::predictedEndTime(double weight)
{
    int averageLength = QTime(0,0,0).secsTo(averageRoundLength(weight));
    return QTime::currentTime().addSecs(averageLength*roundsToPlay());
}

QTime Game::predictedGameLength(double weight)
{
    return finishedRoundsLength().addSecs(QTime(0,0,0).secsTo(predictedTimeToPlay(weight)));
}

QDateTime Game::endDate() const
{
    return rounds().last()->startTime();
    //TODO: Noch nicht wirklich korrekt...
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
        finish();
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

void Game::finish()
{
    QSharedPointer<Round> r = currentRound();
    Q_ASSERT(r);

    removeRound(r);
    Qp::remove<Round>(r);
    r->deleteLater();

    r = currentRound();
    Q_ASSERT(r);

    r->setState(Round::Finished);

    emit stateChanged();
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
    if(m_name.isEmpty()) {
        if(m_type == Doppelkopf) {
            return "Doppelkopf - " + m_creationTime.toString("dd.MM.yyyy hh:mm");
        }
    }
    return m_name;
}

void Game::setName(const QString &name)
{
    m_name = name;
}

QSharedPointer<Place> Game::site() const
{
    return m_site;
}

void Game::setSite(QSharedPointer<Place> site)
{
    m_site = site;
}

QList<QSharedPointer<Player> > Game::players() const
{
    return m_players;
}

void Game::addPlayer(QSharedPointer<Player> player)
{
    m_players.add(player);
}

void Game::removePlayer(QSharedPointer<Player> player)
{
    m_players.remove(player);
}

QSharedPointer<Player> Game::currentCardMixer()
{
    if(isComplete())
        return QSharedPointer<Player>();

    QSharedPointer<Round> round = currentRound();
    if(!round)
        return QSharedPointer<Player>();

    return round->cardMixer();
}

QList<QSharedPointer<Player> > Game::currentPlayingPlayers()
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
    m_players = players;
}

QList<QSharedPointer<Round> > Game::rounds() const
{
    return m_rounds;
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

int Game::roundsTogether(QSharedPointer<Player> player1, QSharedPointer<Player> player2)
{
    calcInitialStats();

    QString key = QString::number(Qp::primaryKey(player1)) + "-" + QString::number(Qp::primaryKey(player2));
    return m_playerRoundsTogether.value(key).size();
}

int Game::winsTogether(QSharedPointer<Player> player1, QSharedPointer<Player> player2)
{
    calcInitialStats();

    QString key = QString::number(Qp::primaryKey(player1)) + "-" + QString::number(Qp::primaryKey(player2));
    return m_playerRoundWinsTogether.value(key).size();
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

int Game::drinkCount(QSharedPointer<Player> player) const
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
    round->setStartTime(QDateTime::currentDateTime());
    round->setNumber(nextNumber);
    round->setState(Round::Running);
    connectRoundSignals(round);
    addRound(round);
    save();

    emit newRoundStarted();
}

int Game::totalRoundCount()
{
    calcInitialStats();

    return m_totalRoundCount;
}

int Game::finishedRoundCount()
{
    calcInitialStats();

    return m_finishedRoundCount;
}

double Game::completedPercentage()
{
    int total = totalRoundCount();
    if(total == 0)
        return 0;

    return (double) finishedRoundCount() * 100 / (double) total;
}

bool Game::isComplete()
{
    return totalRoundCount() == finishedRoundCount();
}

int Game::roundsToPlay()
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
    QList<QSharedPointer<Round> > rs = rounds();
    foreach(QSharedPointer<Round> round, rs) {
        if(roundCount == 0)
            break;

        --roundCount;
        if(!round->hochzeitPlayer() && !round->soloPlayer() && !round->trumpfabgabePlayer()) {
            ++result;
        }
    }

    // Subtract current round
    if(state() != Game::Finished && roundCount >= rs.size())
        --result;

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
        result += round->schmeissereis().size();
    }
    return result;
}

int Game::reWinsCount()
{
    calcInitialStats();

    return m_reWinCount;
}

int Game::contraWinCount()
{
    calcInitialStats();

    return m_contraWinCount;
}

int Game::reGamePoints()
{
    calcInitialStats();

    return m_reGamePoints;
}

int Game::contraGamePoints()
{
    calcInitialStats();

    return m_contraGamePoints;
}

QList<QSharedPointer<Round> > Game::rounds(QSharedPointer<Player> player)
{
    calcInitialStats();

    QList<QSharedPointer<Round> > rounds;
    rounds.append(reRounds(player));
    rounds.append(contraRounds(player));

    return rounds;
}

QList<QSharedPointer<Round> > Game::reRounds(QSharedPointer<Player> player)
{
    calcInitialStats();

    return m_playerReRounds.value(player);
}

QList<QSharedPointer<Round> > Game::contraRounds(QSharedPointer<Player> player)
{
    calcInitialStats();

    return m_playerContraRounds.value(player);
}

QList<QSharedPointer<OLD_OfflineGameInformation> > Game::offlineGameInformation() const
{
    return m_offlineGameInformation;
}

int Game::placement(QSharedPointer<Player> player)
{
    calcInitialStats();
    return m_placements.value(player);
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
    m_rounds = rounds;
    m_currentRoundCached = QSharedPointer<Round>();
}

void Game::addRound(QSharedPointer<Round> round)
{
    m_rounds.add(round);
    m_currentRoundCached = QSharedPointer<Round>();
}

void Game::removeRound(QSharedPointer<Round> round)
{
    m_rounds.remove(round);
    m_currentRoundCached = QSharedPointer<Round>();
}

void Game::setMitPflichtSolo(bool arg)
{
    m_mitPflichtSolo = arg;
}

QString Game::playerString()
{
    QString value = "";
    foreach(QSharedPointer<Player> player, players()) {
        value += player->name() + ",";
    }
    value = value.remove(value.size()-1, 1);

    return value;
}

Game::AdditionalMissingPlayer Game::aditionalMissingPlayer() const
{
    return m_additionalMissingPlayer;
}

void Game::setAdditionalMissingPlayer(Game::AdditionalMissingPlayer arg)
{
    m_additionalMissingPlayer = arg;
}

bool Game::mitPflichtSolo() const
{
    return m_mitPflichtSolo;
}

bool sortGamesByDate(const QSharedPointer<Game> &g1, const QSharedPointer<Game> &g2)
{
    return g1->endDate() < g2->endDate();
}

bool sortGamesByDateLastFirst(const QSharedPointer<Game> &g1, const QSharedPointer<Game> &g2)
{
    return g1->creationTime() > g2->creationTime();
}

void Game::setOfflineGameInformation(const QList<QSharedPointer<OLD_OfflineGameInformation> > &games)
{
    m_offlineGameInformation = games;
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

void Game::calcInitialStats()
{
    if(m_initialCalcedStats) {
        return;
    }

    m_initialCalcedStats = true;

    m_finishedRoundCount = 0;

    m_reWinCount = 0;
    m_contraWinCount = 0;
    m_reGamePoints = 0;
    m_contraGamePoints = 0;

    m_playerReRounds.clear();
    m_playerContraRounds.clear();

    m_playerRoundsTogether.clear();
    m_playerRoundWinsTogether.clear();

    m_totalRoundCount = players().size() * 6;

    QList<QSharedPointer<Round>> _rounds = rounds();
    qSort(_rounds.begin(), _rounds.end(), sortRoundsByNumber);
    setRounds(_rounds);

    QSharedPointer<Round> lastFinishedRound;
    foreach(QSharedPointer<Round> round, _rounds) {
        if(round->state() == Round::Finished) {

            m_finishedRoundCount++;
            lastFinishedRound = round;

            if(round->winnerParty() == Round::Re) {
                m_reWinCount++;
            }
            else if(round->winnerParty() == Round::Contra) {
                m_contraWinCount++;
            }

            if(this->creationTime().date() > QDate(2014,1,21)) {
                m_reGamePoints += round->reGamePoints();
                m_contraGamePoints += round->contraGamePoints();
            }

            QSharedPointer<Player> re1Player = round->re1Player();
            addToRePlayersStats(round, re1Player);

            QSharedPointer<Player> re2Player = round->re2Player();
            addToRePlayersStats(round, re2Player);

            QSharedPointer<Player> contra1Player = round->contra1Player();
            addToContraPlayersStats(round, contra1Player);

            QSharedPointer<Player> contra2Player = round->contra2Player();
            addToContraPlayersStats(round, contra2Player);

            QSharedPointer<Player> contra3Player = round->contra3Player();
            addToContraPlayersStats(round, contra3Player);

            if(round->isSolo()) {
                addToGamesTogetherStats(round, contra1Player, contra2Player);
                addToGamesTogetherStats(round, contra3Player, contra2Player);
                addToGamesTogetherStats(round, contra3Player, contra1Player);
            }
            else {
                addToGamesTogetherStats(round, re1Player, re2Player);
                addToGamesTogetherStats(round, contra1Player, contra2Player);
            }
        }
    }

    if(!lastFinishedRound) {
        return;
    }

    foreach(QSharedPointer<Player> player, players()) {
        m_placements.insert(player, lastFinishedRound->placement(player));
    }
}

void Game::updateStats()
{
    if(!m_initialCalcedStats) {
        calcInitialStats();
    }

    QList<QSharedPointer<Round> > rs = rounds();
    if(rs.size() < 2) { // es gibt nur die eine Runde, die gerade läuft
        return;
    }

    QSharedPointer<Round> finishedRound = rs.at(rs.size()-2);
    if(finishedRound->state() != Round::Finished) {
        return;
    }
    m_finishedRoundCount++;

    if(finishedRound->winnerParty() == Round::Re) {
        m_reWinCount++;
    }
    else if(finishedRound->winnerParty() == Round::Contra) {
        m_contraWinCount++;
    }

    if(this->creationTime().date() > QDate(2014,1,21)) {
        m_reGamePoints += finishedRound->reGamePoints();
        m_contraGamePoints += finishedRound->contraGamePoints();
    }

    QSharedPointer<Player> re1Player = finishedRound->re1Player();
    addToRePlayersStats(finishedRound, re1Player);

    QSharedPointer<Player> re2Player = finishedRound->re2Player();
    addToRePlayersStats(finishedRound, re2Player);

    QSharedPointer<Player> contra1Player = finishedRound->contra1Player();
    addToContraPlayersStats(finishedRound, contra1Player);

    QSharedPointer<Player> contra2Player = finishedRound->contra2Player();
    addToContraPlayersStats(finishedRound, contra2Player);

    QSharedPointer<Player> contra3Player = finishedRound->contra3Player();
    addToContraPlayersStats(finishedRound, contra3Player);

    if(finishedRound->isSolo()) {
        addToGamesTogetherStats(finishedRound, contra1Player, contra2Player);
        addToGamesTogetherStats(finishedRound, contra3Player, contra2Player);
        addToGamesTogetherStats(finishedRound, contra3Player, contra1Player);
    }
    else {
        addToGamesTogetherStats(finishedRound, re1Player, re2Player);
        addToGamesTogetherStats(finishedRound, contra1Player, contra2Player);
    }

    foreach(QSharedPointer<Player> player, players()) {
        m_placements.insert(player, finishedRound->placement(player));
    }
}

void Game::addToRePlayersStats(QSharedPointer<Round> round, QSharedPointer<Player> player)
{
    if(player) {
        QList<QSharedPointer<Round> > reRounds;
        if(m_playerReRounds.contains(player)) {
            reRounds.append(m_playerReRounds.value(player));
        }
        reRounds.append(round);
        m_playerReRounds.insert(player, reRounds);
    }
}

void Game::addToContraPlayersStats(QSharedPointer<Round> round, QSharedPointer<Player> player)
{
    if(player) {
        QList<QSharedPointer<Round> > reRounds;
        if(m_playerContraRounds.contains(player)) {
            reRounds.append(m_playerContraRounds.value(player));
        }
        reRounds.append(round);
        m_playerContraRounds.insert(player, reRounds);
    }
}

void Game::addToGamesTogetherStats(QSharedPointer<Round> round, QSharedPointer<Player> player1, QSharedPointer<Player> player2)
{
    if(!round || !player1 || !player2)
        return;

    QString key = QString::number(Qp::primaryKey(player1)) + "-" + QString::number(Qp::primaryKey(player2));
    QString key1 = QString::number(Qp::primaryKey(player2)) + "-" + QString::number(Qp::primaryKey(player1));

    QList<QSharedPointer<Round> > rounds;
    if(m_playerRoundsTogether.contains(key)) {
        rounds.append(m_playerRoundsTogether.value(key));
    }
    else if(m_playerRoundsTogether.contains(key1)) {
        rounds.append(m_playerRoundsTogether.value(key1));
    }

    rounds.append(round);

    m_playerRoundsTogether.insert(key, rounds);
    m_playerRoundsTogether.insert(key1, rounds);

    if(round->points(player1) > 0) {
        QList<QSharedPointer<Round> > wins;
        if(m_playerRoundWinsTogether.contains(key)) {
            wins.append(m_playerRoundWinsTogether.value(key));
        }
        else if(m_playerRoundWinsTogether.contains(key1)) {
            wins.append(m_playerRoundWinsTogether.value(key1));
        }
        wins.append(round);

        m_playerRoundWinsTogether.insert(key, wins);
        m_playerRoundWinsTogether.insert(key1, wins);
    }

}

bool sortRoundsByNumber(const QSharedPointer<Round> &g1, const QSharedPointer<Round> &g2)
{
    return g1->number() < g2->number();
}
