#include "round.h"

#include "game.h"
#include "livedrink.h"
#include "player.h"
#include "schmeisserei.h"

#include <limits>

#include "parsecontroller.h"

Round::Round(QObject *parent) :
    ParseObject(parent),
    m_number(-1),
    m_state(Round::UnkownState),
    m_length(QTime(0,0,0)),
    m_soloType(UnkownSoloType),
    m_soloIsPflicht(false),
    m_winnerParty(UnknownWinnerParty),
    m_trumpfColor(UnknownTrumpColor),
    m_hochzeitDecision(UnkownHochzeitDecision),
    m_trumpfCount(-1),
    m_trumpfZurueck(false),
    m_reGamePoints(120),
    m_contraGamePoints(120),
    m_game(QpRelation(&Round::game)),
    m_liveDrinks(QpRelation(&Round::liveDrinks)),
    m_schmeissereis(QpRelation(&Round::schmeissereis)),
    m_hochzeitPlayer(QpRelation(&Round::hochzeitPlayer)),
    m_trumpfabgabePlayer(QpRelation(&Round::trumpfabgabePlayer)),
    m_soloPlayer(QpRelation(&Round::soloPlayer)),
    m_schweinereiPlayer(QpRelation(&Round::schweinereiPlayer)),
    m_re1Player(QpRelation(&Round::re1Player)),
    m_re2Player(QpRelation(&Round::re2Player)),
    m_contra1Player(QpRelation(&Round::contra1Player)),
    m_contra2Player(QpRelation(&Round::contra2Player)),
    m_contra3Player(QpRelation(&Round::contra3Player))
{
}

Round::~Round()
{
}

QList<QSharedPointer<LiveDrink> > Round::liveDrinks() const
{
    return m_liveDrinks;
}

void Round::addLiveDrink(QSharedPointer<LiveDrink> drink)
{
    m_liveDrinks.add(drink);

    emit drinkAdded();
}

void Round::removeLiveDrink(QSharedPointer<LiveDrink> drink)
{
    m_liveDrinks.remove(drink);

    emit drinkRemoved();
}

void Round::setLiveDrinks(const QList<QSharedPointer<LiveDrink> > &drinks)
{
    m_liveDrinks = drinks;
}

QSharedPointer<Game> Round::game() const
{
    return m_game;
}

void Round::setGame(const QSharedPointer<Game> &game)
{
    m_game = game;
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

Round::Type Round::type() const
{
    if(soloPlayer()) {
        return Solo;
    }
    else if(hochzeitPlayer()) {
        return Hochzeit;
    }
    else if(trumpfabgabePlayer()) {
        return Trumpfabgabe;
    }

    return NormalRound;
}

QString Round::typeString() const
{
    switch(type()) {
    case Solo:
        return soloTypeString();
    case Hochzeit:
        return "Hochzeit";
    case Trumpfabgabe:
        return "Trumpfabgabe";
    case NormalRound:
        return "Normal round";

    }

    return "";
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

    // Invalidate total points cache of following rows
    QList<QSharedPointer<Round> > rounds = game()->rounds();
    int n = number();
    int size = rounds.size();
    for(int i = n; i < size; ++i) {
        rounds.at(i)->m_totalPointsCached.remove(player);
    }
}

int Round::totalPoints(QSharedPointer<Player> player) const
{
    auto it = m_totalPointsCached.find(player);
    if(it != m_totalPointsCached.end())
        return it.value();

    int result = 0;
    QList<QSharedPointer<Round> > rounds = game()->rounds();
    int n = number();
    for(int i = 0; i <= n; ++i) {
        result += rounds.at(i)->points(player);
    }

    m_totalPointsCached.insert(player, result);
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
    QList<QSharedPointer<Player> > ps = game()->players();
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

    return playersByPosition();
}

QList<QSharedPointer<Player> > Round::playersByPosition() const
{
    QList<QSharedPointer<Player> > result;
    QList<QSharedPointer<Player> > ps = game()->players();
    if(ps.isEmpty())
        return QList<QSharedPointer<Player> >();

    int countPlayers = ps.size();
    int cardMixerPos = cardMixerPosition();
    int zusaetzlichAussetzenderSpieler = number() / countPlayers;
    if(game()->aditionalMissingPlayer() == Game::MissingOppositeOfCardMixer)
        zusaetzlichAussetzenderSpieler = countPlayers / 2 - 1;

    for(int i = cardMixerPos + 1, anzahl = 0; anzahl < 4; ++i, ++anzahl)
    {
        i = i % countPlayers;

        if(countPlayers > 5 && anzahl == zusaetzlichAussetzenderSpieler)
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
    return m_re2Player;
}

void Round::setRe2Player(const QSharedPointer<Player> &re2Player)
{
    m_re2Player = re2Player;
}

bool Round::isRe(QSharedPointer<Player> player) const
{
    return rePlayers().contains(player);
}

QSharedPointer<Player> Round::re1Player() const
{
    return m_re1Player;
}

void Round::setRe1Player(const QSharedPointer<Player> &re1Player)
{
    m_re1Player = re1Player;
}

QSharedPointer<Player> Round::schweinereiPlayer() const
{
    return m_schweinereiPlayer;
}

void Round::setSchweinereiPlayer(const QSharedPointer<Player> &schweinereiPlayer)
{
    m_schweinereiPlayer = schweinereiPlayer;
}

QSharedPointer<Player> Round::soloPlayer() const
{
    return m_soloPlayer;
}

void Round::setSoloPlayer(const QSharedPointer<Player> &soloPlayer)
{
    m_soloPlayer = soloPlayer;
}

QSharedPointer<Player> Round::trumpfabgabePlayer() const
{
    return m_trumpfabgabePlayer;
}

void Round::setTrumpfabgabePlayer(const QSharedPointer<Player> &trumpfabgabePlayer)
{
    m_trumpfabgabePlayer = trumpfabgabePlayer;
}

QSharedPointer<Player> Round::hochzeitPlayer() const
{
    return m_hochzeitPlayer;
}

void Round::setHochzeitPlayer(const QSharedPointer<Player> &hochzeitPlayer)
{
    m_hochzeitPlayer = hochzeitPlayer;
}

QList<QSharedPointer<Schmeisserei> > Round::schmeissereis() const
{
    return m_schmeissereis;
}

void Round::addSchmeisserei(QSharedPointer<Schmeisserei> schmeisserei)
{
    m_schmeissereis.add(schmeisserei);

    emit schmeissereiAdded();
}

void Round::setSchmeissereis(const QList<QSharedPointer<Schmeisserei> > &schmeissereien)
{
    m_schmeissereis = schmeissereien;
}

void Round::removeSchmeisserei(QSharedPointer<Schmeisserei> schmeisserei)
{
    m_schmeissereis.remove(schmeisserei);
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
    return m_contra1Player;
}

void Round::setContra1Player(QSharedPointer<Player> arg)
{
    m_contra1Player = arg;
}

QSharedPointer<Player> Round::contra2Player() const
{
    return m_contra2Player;
}

void Round::setContra2Player(QSharedPointer<Player> arg)
{
    m_contra2Player = arg;
}

QSharedPointer<Player> Round::contra3Player() const
{
    return m_contra3Player;
}

void Round::setContra3Player(QSharedPointer<Player> arg)
{
    m_contra3Player = arg;
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

int Round::trumpfCount() const
{
    return m_trumpfCount;
}

void Round::setTrumpfCount(int arg)
{
    m_trumpfCount = arg;
}

int Round::trumpfZurueck() const
{
    return m_trumpfZurueck;
}

void Round::setTrumpfZurueck(int arg)
{
    m_trumpfZurueck = arg;
}

int Round::reGamePoints() const
{
    return m_reGamePoints;
}

void Round::setReGamePoints(int arg)
{
    m_reGamePoints = arg;
}

int Round::contraGamePoints() const
{
    return m_contraGamePoints;
}

void Round::setContraGamePoints(int arg)
{
    m_contraGamePoints = arg;
}

QString Round::name() const
{
    return QString::number(number()) + " - " + startTime().toString("dd.MM.yyyy hh:mm");
}


QByteArray Round::parseJSONData()
{
    QByteArray postData;
    postData.append("{");
    postData.append(QString("\"gameID\"") + ":" + "\"" + this->game()->parseID() + "\"");
    postData.append(",");
    postData.append(QString("\"number\"") + ":" + QString::number(this->number()));
    postData.append(",");
    postData.append(QString("\"startTime\"") + ":" + "{\"__type\": \"Date\",\"iso\": \"" + this->startTime().toString(Qt::ISODate) + "\"}");
    postData.append(",");
    postData.append(QString("\"reGamePoints\"") + ":" + QString::number(this->reGamePoints()));
    postData.append(",");
    postData.append(QString("\"contraGamePoints\"") + ":" + QString::number(this->contraGamePoints()));
    postData.append(",");
    postData.append(QString("\"state\"") + ":" + QString::number(this->state()));
    postData.append(",");
    postData.append(QString("\"winnerParty\"") + ":" + QString::number(this->winnerParty()));
    postData.append(",");
    postData.append(QString("\"hochzeitDecision\"") + ":" + QString::number(this->hochzeitDecision()));
    postData.append(",");
    QString commentString = this->comment().toUtf8();
    commentString.replace("\n", "\\n");
    postData.append(QString("\"comment\"") + ":" + "\"" + commentString + "\"");
    postData.append(",");
    if(this->isPflicht()) {
        postData.append(QString("\"isPflicht\"") + ":" + "true");
    }
    else {
        postData.append(QString("\"isPflicht\"") + ":" + "false");
    }
    postData.append(",");
    postData.append(QString("\"trumpfCount\"") + ":" + QString::number(this->trumpfCount()));
    postData.append(",");
    postData.append(QString("\"trumpfZurueck\"") + ":" + QString::number(this->trumpfZurueck()));

    QList<QSharedPointer<ParseObject>> list;
    if(liveDrinks().size() > 0) {
        postData.append(",");
        foreach(QSharedPointer<LiveDrink> drink, liveDrinks()) {
            list << drink;
        }
        postData.append(QString("\"liveDrinks\":") + parseJSONString(list));
    }
    if(schmeissereis().size() > 0) {
        list.clear();
        postData.append(",");
        foreach(QSharedPointer<Schmeisserei> schmeisserei, schmeissereis()) {
            list << schmeisserei;
        }
        postData.append(QString("\"schmeissereis\":") + parseJSONString(list));
    }

    if(state() == Finished) {
        postData.append(",");
        postData.append(QString("\"re1Player\"") + ":" + parseJSONString(this->re1Player()));
        postData.append(",");
        if(this->isSolo()) {
            if(game()->type() == Game::Doppelkopf) {
                postData.append(QString("\"contra3Player\"") + ":" + parseJSONString(this->contra3Player()));
                postData.append(",");
            }
            postData.append(QString("\"soloPlayer\"") + ":" + parseJSONString(this->soloPlayer()));
            postData.append(",");
            postData.append(QString("\"soloType\"") + ":" + QString::number(this->soloType()));
        }
        else {
            postData.append(QString("\"re2Player\"") + ":" + parseJSONString(this->re2Player()));
        }
        postData.append(",");
        postData.append(QString("\"contra1Player\"") + ":" + parseJSONString(this->contra1Player()));
        postData.append(",");
        postData.append(QString("\"contra2Player\"") + ":" + parseJSONString(this->contra2Player()));
        if(this->schweinereiPlayer()) {
            postData.append(",");
            postData.append(QString("\"schweinereiPlayer\"") + ":" + parseJSONString(this->schweinereiPlayer()));
        }
        if(this->hochzeitPlayer()) {
            postData.append(",");
            postData.append(QString("\"hochzeitPlayer\"") + ":" + parseJSONString(this->hochzeitPlayer()));
        }
        if(this->trumpfabgabePlayer()) {
            postData.append(",");
            postData.append(QString("\"trumpfabgabePlayer\"") + ":" + parseJSONString(this->trumpfabgabePlayer()));
        }
        postData.append(",");

        QString pointsString = "";
        pointsString += QString("\"points\"") + ":" + "{";
        foreach(QSharedPointer<Player> player, this->playingPlayers()) {
            pointsString += "\"" + player->parseID() + "\":" + QString::number(this->points(player)) + ",";
        }
        pointsString.remove(pointsString.size()-1,1);
        postData.append(pointsString);
        postData.append("}");
    }

    postData.append("}");

    return postData;
}

void Round::parseUpdateFromJSON(QJsonObject object, bool created)
{
    QSharedPointer<Game> game = ParseController::instance()->objectFromCache<Game>(object.value("gameID").toString());
    if(!game) {
        qDebug() << "ERROR fetching round-data: " << object;
        return;
    }

    if(created) {
        game->addRound(Qp::sharedFrom(this));
    }
    Qp::update(Qp::sharedFrom(this));
    Qp::update(game);

    setNumber(object.value("number").toInt());
    setState(static_cast<State>(object.value("state").toInt()));

    QString dateString = object.value("startTime").toObject().value("iso").toString();
    setStartTime(QDateTime::fromString(dateString, Qt::ISODate));

    setReGamePoints(object.value("reGamePoints").toInt());
    setContraGamePoints(object.value("contraGamePoints").toInt());
    setWinnerParty(static_cast<WinnerParty>(object.value("winnerParty").toInt()));
    setHochzeitDecision(static_cast<HochzeitDecision>(object.value("hochzeitDecision").toInt()));
    setIsPflicht(object.value("isPflicht").toBool());
    setTrumpfCount(object.value("trumpfCount").toInt());
    setTrumpfZurueck(object.value("trumpfZurueck").toInt());
    setSoloType(static_cast<SoloType>(object.value("soloType").toInt()));

    if(state() == Round::Finished) {
        QSharedPointer<Player> player = objectFromJSONPointer<Player>(object.value("re1Player").toObject());
        Q_ASSERT(player);
        setRe1Player(player);

        if(object.contains("re2Player")) {
            player = objectFromJSONPointer<Player>(object.value("re2Player").toObject());
            Q_ASSERT(player);
            setRe2Player(player);
        }

        player = objectFromJSONPointer<Player>(object.value("contra1Player").toObject());
        Q_ASSERT(player);
        setContra1Player(player);

        player = objectFromJSONPointer<Player>(object.value("contra2Player").toObject());
        Q_ASSERT(player);
        setContra2Player(player);

        if(object.contains("contra3Player")) {
            player = objectFromJSONPointer<Player>(object.value("contra3Player").toObject());
            Q_ASSERT(player);
            setContra3Player(player);
        }

        if(object.contains("soloPlayer")) {
            player = objectFromJSONPointer<Player>(object.value("soloPlayer").toObject());
            Q_ASSERT(player);
            setSoloPlayer(player);
        }

        if(object.contains("hochzeitPlayer")) {
            player = objectFromJSONPointer<Player>(object.value("hochzeitPlayer").toObject());
            Q_ASSERT(player);
            setHochzeitPlayer(player);
        }

        if(object.contains("trumpfabgabePlayer")) {
            player = objectFromJSONPointer<Player>(object.value("trumpfabgabePlayer").toObject());
            Q_ASSERT(player);
            setTrumpfabgabePlayer(player);
        }

        if(object.contains("schweinereiPlayer")) {
            player = objectFromJSONPointer<Player>(object.value("schweinereiPlayer").toObject());
            Q_ASSERT(player);
            setSchweinereiPlayer(player);
        }

        QJsonObject pointsObject = object.value("points").toObject();
        foreach(QString pointValueKey, pointsObject.keys()) {
            player = ParseController::instance()->objectFromCache<Player>(pointValueKey);
            Q_ASSERT(player);
            setPoints(player, pointsObject.value(pointValueKey).toInt());
        }
    }

    Qp::update(Qp::sharedFrom(this));
    Qp::update(game);

    game->emitParseUpdated();
}


bool Round::parseCheckPreUploadConditions()
{
    if(game()->parseID() == "") {
        game()->parseUpload();
        return false;
    }

    return true;
}

bool Round::parseCheckAfterUploadConditions()
{
    foreach(QSharedPointer<Schmeisserei> schmeisserei, schmeissereis()) {
        if(schmeisserei->parseID() == "" || !schmeisserei->parseUpdated()) {
            schmeisserei->parseUpload();
            return false;
        }
    }

    foreach(QSharedPointer<LiveDrink> drink, liveDrinks()) {
        if(drink->parseID() == "" || !drink->parseUpdated()) {
            drink->parseUpload();
            return false;
        }
    }

    m_isUploading = false;
    ParseObject::currentUploadingObjects.remove("Round");

    if(game()->parseCheckAfterUploadConditions()) {
        game()->parseUpload();
    }

    return true;
}
