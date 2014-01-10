#include "round.h"

#include "game.h"
#include "livedrink.h"
#include "player.h"
#include "schmeisserei.h"

#include <limits>



Round::Round(QObject *parent) :
    QObject(parent),
    m_number(-1),
    m_state(Round::UnkownState),
    m_length(QTime(0,0,0)),
    m_soloType(UnkownSoloType),
    m_soloIsPflicht(false),
    m_winnerParty(UnknownWinnerParty),
    m_game("game", this),
    m_drinks("drinks",this),
    m_schmeissereien("schmeissereien",this),
    m_hochzeitPlayer("hochzeitPlayer",this),
    m_trumpfabgabePlayer("trumpfabgabePlayer",this),
    m_soloPlayer("soloPlayer",this),
    m_schweinereiPlayer("schweinereiPlayer",this),
    m_re1Player("re1Player",this),
    m_re2Player("re2Player",this),
    m_contra1Player("contra1Player",this),
    m_contra2Player("contra2Player",this),
    m_contra3Player("contra3Player",this),
    m_hochzeitDecision(Round::UnkownHochzeitDecision)
{
}

Round::~Round()
{
}

QList<QSharedPointer<LiveDrink> > Round::drinks() const
{
    return m_drinks.resolveList();
}

void Round::addDrink(QSharedPointer<LiveDrink> drink)
{
    m_drinks.relate(drink);
}

void Round::setDrinks(const QList<QSharedPointer<LiveDrink> > &drinks)
{
    m_drinks.clear();
    m_drinks.relate(drinks);
}

QSharedPointer<Game> Round::game() const
{
    return m_game.resolve();
}

void Round::setGame(const QSharedPointer<Game> &game)
{
    m_game.relate(game);
}

QMap<int, int> Round::_points() const
{
    return m_points;
}

void Round::_setPoints(const QMap<int, int> &points)
{
    m_points = points;
}

QString Round::comment() const
{
    return m_comment;
}

void Round::setComment(const QString &comment)
{
    m_comment = comment;
}

int Round::points() const
{
    QSharedPointer<Player> player = re1Player();

    if(!player)
        return 0;

    int p = points(player);
    if(isSolo())
        p /= 3;
    return p;
}

int Round::points(QSharedPointer<Player> player) const
{
    auto it = m_pointsCached.find(player);
    if(it == m_pointsCached.end()) {
        int points = _points().value(Qp::primaryKey(player));
        m_pointsCached.insert(player, points);
        return points;
    }

    return it.value();
}

void Round::setPoints(QSharedPointer<Player> player, int points)
{
    QMap<int,int> ps = _points();
    ps.insert(Qp::primaryKey(player), points);
    m_pointsCached.insert(player, points);
    _setPoints(ps);
}

int Round::totalPoints(QSharedPointer<Player> player) const
{
    int result = 0;

    QList<QSharedPointer<Round> > rounds = game()->rounds();
    int n = number();
    for(int i = 0; i <= n; ++i) {
        result += rounds.at(i)->points(player);
    }

    return result;
}

QList<QSharedPointer<Player> > Round::playersSortedByPlacement() const
{
    QList<QSharedPointer<Player> > ps = game()->players();

    QMultiMap<int, QSharedPointer<Player>> helperMap;
    foreach(QSharedPointer<Player> player, ps) {
        helperMap.insert(totalPoints(player), player);
    }

    if(game()->type() == Game::Doppelkopf || game()->type() == Game::Prognose) { // games with most point winning
        return Qp::reversed(helperMap.values());
    }
    else{ // games with less points winning
        return helperMap.values();
    }
}

int Round::placement(QSharedPointer<Player> player) const
{
    if(!game()->players().contains(player))
        return -1;

    int place = 1;
    QList<QSharedPointer<Player> > ps = playersSortedByPlacement();
    int points = totalPoints(player);
    foreach(QSharedPointer<Player> p, ps) {
        if(game()->type() == Game::Doppelkopf || game()->type() == Game::Prognose) { // games with most point winning
            if(totalPoints(p) > points) {
                ++place;
            }
        }
        else{
            if(totalPoints(p) < points) {
                ++place;
            }
        }
    }

    return place;
}

int Round::pointsToLeader(QSharedPointer<Player> player)
{
    if(!playersSortedByPlacement().contains(player)) {
        return 0;
    }

    int leaderPoints = totalPoints(playersSortedByPlacement().first());
    int playerPoints = totalPoints(player);

    return leaderPoints - playerPoints;
}

QSharedPointer<Player> Round::cardMixer() const
{
    int cardMixerPos = cardMixerPosition();
    if(cardMixerPos < 0)
        return QSharedPointer<Player>();

    QList<QSharedPointer<Player> > ps = game()->players();
    if(ps.size() <= cardMixerPos)
        return QSharedPointer<Player>();

    return ps.at(cardMixerPos);
}

QList<QSharedPointer<Player> > Round::playingPlayers() const
{
    QList<QSharedPointer<Player> > result;
    result.append(rePlayers());
    result.append(contraPlayers());
    if(result.size() == 4)
        return result;

    result.clear();

    QList<QSharedPointer<Player> > ps = game()->players();
    if(ps.isEmpty())
        return QList<QSharedPointer<Player> >();

    int countPlayers = ps.size();
    int cardMixerPos = cardMixerPosition();
    for(int i = cardMixerPos + 1, anzahl = 0; anzahl < 4; ++i, ++anzahl)
    {
        i = i % countPlayers;

        if(countPlayers > 5 && anzahl+1 == countPlayers / 2)
            ++i;

        i = i % countPlayers;

        QSharedPointer<Player> player = ps.at(i);
        result.append(player);
    }

    Q_ASSERT(result.size() == 4);
    return result;
}

int Round::cardMixerPosition() const
{
    int countPlayers = game()->players().size();
    if(countPlayers == 0)
        return -1;

    return number() % countPlayers;
}

int Round::_state() const
{
    return static_cast<int>(state());
}

void Round::_setState(int state)
{
    setState(static_cast<Round::State>(state));
}

int Round::_soloType() const
{
    return static_cast<int>(soloType());
}

void Round::_setSoloType(int type)
{
    setSoloType(static_cast<Round::SoloType>(type));
}

int Round::_winnerParty() const
{
    return static_cast<int>(winnerParty());
}

void Round::_setWinnerParty(int party)
{
    setWinnerParty(static_cast<Round::WinnerParty>(party));
}

int Round::_trumpfColor() const
{
    return static_cast<int>(trumpfColor());
}

void Round::_setTrumpfColor(int color)
{
    setTrumpfColor(static_cast<Round::TrumpfColor>(color));
}


QTime Round::length() const
{
    return m_length;
}

void Round::setLength(const QTime &length)
{
    if(m_length.second() == 59)
        Qp::update(Qp::sharedFrom(this));

    m_length = length;
}

QDateTime Round::startTime() const
{
    return m_startTime;
}

void Round::setStartTime(const QDateTime &startTime)
{
    m_startTime = startTime;
}

int Round::number() const
{
    return m_number;
}

void Round::setNumber(int number)
{
    m_number = number;
}

Round::WinnerParty Round::winnerParty() const
{
    return m_winnerParty;
}

void Round::setWinnerParty(const WinnerParty &winnerParty)
{
    m_winnerParty = winnerParty;
}

Round::TrumpfColor Round::trumpfColor() const
{
    return m_trumpfColor;
}

void Round::setTrumpfColor(const Round::TrumpfColor &trumpfColor)
{
    m_trumpfColor = trumpfColor;
}

bool Round::isPflicht() const
{
    return m_soloIsPflicht;
}

void Round::setIsPflicht(bool soloIsPflicht)
{
    m_soloIsPflicht = soloIsPflicht;
}

Round::SoloType Round::soloType() const
{
    return m_soloType;
}

void Round::setSoloType(const SoloType &soloType)
{
    m_soloType = soloType;
}

QString Round::soloTypeString() const
{
    return soloTypeStringFromType(soloType());
}

bool Round::isSolo() const
{
    return soloType() != NoSolo && soloType() != UnkownSoloType;
}

QSharedPointer<Player> Round::re2Player() const
{
    return m_re2Player.resolve();
}

void Round::setRe2Player(const QSharedPointer<Player> &re2Player)
{
    m_re2Player.relate(re2Player);
}

bool Round::isRe(QSharedPointer<Player> player) const
{
    return rePlayers().contains(player);
}

QSharedPointer<Player> Round::re1Player() const
{
    return m_re1Player.resolve();
}

void Round::setRe1Player(const QSharedPointer<Player> &re1Player)
{
    m_re1Player.relate(re1Player);
}

QSharedPointer<Player> Round::schweinereiPlayer() const
{
    return m_schweinereiPlayer.resolve();
}

void Round::setSchweinereiPlayer(const QSharedPointer<Player> &schweinereiPlayer)
{
    m_schweinereiPlayer.relate(schweinereiPlayer);
}

QSharedPointer<Player> Round::soloPlayer() const
{
    return m_soloPlayer.resolve();
}

void Round::setSoloPlayer(const QSharedPointer<Player> &soloPlayer)
{
    m_soloPlayer.relate(soloPlayer);
}

QSharedPointer<Player> Round::trumpfabgabePlayer() const
{
    return m_trumpfabgabePlayer.resolve();
}

void Round::setTrumpfabgabePlayer(const QSharedPointer<Player> &trumpfabgabePlayer)
{
    m_trumpfabgabePlayer.relate(trumpfabgabePlayer);
}

QSharedPointer<Player> Round::hochzeitPlayer() const
{
    return m_hochzeitPlayer.resolve();
}

void Round::setHochzeitPlayer(const QSharedPointer<Player> &hochzeitPlayer)
{
    m_hochzeitPlayer.relate(hochzeitPlayer);
}

QList<QSharedPointer<Schmeisserei> > Round::schmeissereien() const
{
    return m_schmeissereien.resolveList();
}

void Round::addSchmeisserei(QSharedPointer<Schmeisserei> schmeisserei)
{
    m_schmeissereien.relate(schmeisserei);
    schmeisserei->setRound(Qp::sharedFrom(this));

    emit schmeissereiAdded();
}

void Round::setSchmeissereien(const QList<QSharedPointer<Schmeisserei> > &schmeissereien)
{
    m_schmeissereien.relate(schmeissereien);
}


QList<QSharedPointer<Player> > Round::winners() const
{
    WinnerParty winner = winnerParty();
    if(winner == Re)
        return rePlayers();
    else if(winner == Contra)
        return contraPlayers();

    return QList<QSharedPointer<Player> >();
}

QList<QSharedPointer<Player> > Round::losers() const
{
    WinnerParty winner = winnerParty();
    if(winner == Re)
        return contraPlayers();
    else if(winner == Contra)
        return rePlayers();

    return QList<QSharedPointer<Player> >();
}


QList<QSharedPointer<Player> > Round::rePlayers() const
{
    QList<QSharedPointer<Player> > result;
    QSharedPointer<Player> p = re1Player();
    if(!p)
        return QList<QSharedPointer<Player> >();
    result.append(p);

    if(isSolo())
        return result;

    p = re2Player();
    if(!p)
        return result;
    result.append(p);
    return result;
}

QList<QSharedPointer<Player> > Round::contraPlayers() const
{    
    QList<QSharedPointer<Player> > result;
    QSharedPointer<Player> p = contra1Player();
    if(!p)
        return QList<QSharedPointer<Player> >();
    result.append(p);

    p = contra2Player();
    if(!p)
        return result;
    result.append(p);

    if(!isSolo())
        return result;

    p = contra3Player();
    if(!p)
        return result;
    result.append(p);
    return result;
}

Round::State Round::state() const
{
    return m_state;
}

void Round::setState(const State &arg)
{
    m_state = arg;
}

QSharedPointer<Player> Round::contra1Player() const
{
    return m_contra1Player.resolve();
}

void Round::setContra1Player(QSharedPointer<Player> arg)
{
    m_contra1Player.relate(arg);
}

QSharedPointer<Player> Round::contra2Player() const
{
    return m_contra2Player.resolve();
}

void Round::setContra2Player(QSharedPointer<Player> arg)
{
    m_contra2Player.relate(arg);
}

QSharedPointer<Player> Round::contra3Player() const
{
    return m_contra3Player.resolve();
}

void Round::setContra3Player(QSharedPointer<Player> arg)
{
    m_contra3Player.relate(arg);
}

QStringList Round::soloTypeStrings()
{
    return QStringList() << tr("Fleischlos") <<
                            tr("Buben") <<
                            tr("Damen") <<
                            tr("Trumpf") <<
                            tr("Stille Hochzeit") <<
                            tr("Sitzengelassene Hochzeit") <<
                            tr("Falsch gespielt") <<
                            tr("Farb");
}

QString Round::soloTypeStringFromType(Round::SoloType type)
{
    int typeIndex = static_cast<int>(type);
    typeIndex -= 2; // UnkownSolo and NoSolo
    QStringList types = soloTypeStrings();
    if(typeIndex < 0 || typeIndex >= types.size())
        return tr("Unkown solo");

    return types.at(typeIndex);
}

Round::SoloType Round::soloTypeFromString(const QString &typeString)
{
    int typeIndex = soloTypeStrings().indexOf(typeString);
    if(typeIndex < 0)
        return Round::NoSolo;

    typeIndex += 2; // UnkownSolo and NoSolo
    return static_cast<Round::SoloType>(typeIndex);
}

Round::HochzeitDecision Round::hochzeitDecision() const
{
    return m_hochzeitDecision;
}


void Round::setHochzeitDecision(Round::HochzeitDecision arg)
{
    if(arg > Round::ErsterTrumpf)
        arg = Round::UnkownHochzeitDecision;

    m_hochzeitDecision = arg;
}
