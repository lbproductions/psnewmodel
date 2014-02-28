#include "player.h"

#include "place.h"
#include "livedrink.h"
#include "game.h"
#include "round.h"
#include "schmeisserei.h"
#include "playerstatistics.h"
#include "league.h"
#include "old_offlineGameInformation.h"

#include <QPersistence.h>
#include <QPainter>

Player::Player(QObject *parent) :
    QObject(parent),
    m_gender(UnkownGender),
    m_weight(-1),
    m_height(-1),
    m_allGamesStatistics(new PlayerStatistics()),
    m_places("places", this),
    m_liveDrinks("liveDrinks", this),
    m_leagues("leagues", this),
    m_games("games",this),
    m_schmeissereien("schmeissereien",this),
    m_hochzeitRounds("hochzeitRounds",this),
    m_trumpfabgabeRounds("trumpfabgabeRounds",this),
    m_soloRounds("soloRounds",this),
    m_schweinereiRounds("schweinereiRounds",this),
    m_re1Rounds("re1Rounds",this),
    m_re2Rounds("re2Rounds",this),
    m_contra1Rounds("contra1Rounds",this),
    m_contra2Rounds("contra2Rounds",this),
    m_contra3Rounds("contra3Rounds",this),
    m_offlineGameInformation("offlineGameInformation", this)
{
    m_allGamesStatistics->setPlayer(this);
}

Player::~Player()
{
}

QPixmap Player::avatar() const
{
    return m_avatar;
}

void Player::setAvatar(const QPixmap &avatar)
{
    m_avatar = avatar;
}

QColor Player::color() const
{
    return m_color;
}

void Player::setColor(const QColor &color)
{
    m_color = color;
}

QPixmap Player::colorPixmap(int w, int h) const
{
    QPixmap pixmap(w,h);
    QPainter painter(&pixmap);
    painter.setBrush(color());
    painter.setPen(QColor(108,108,108));
    painter.drawRect(QRect(0,0,w-1,h-1));
    return pixmap;
}

int Player::height() const
{
    return m_height;
}

void Player::setHeight(int size)
{
    m_height = size;
}

int Player::weight() const
{
    return m_weight;
}

void Player::setWeight(int weight)
{
    m_weight = weight;
}

Player::Gender Player::gender() const
{
    return m_gender;
}

void Player::setGender(const Gender &gender)
{
    m_gender = gender;
}

QList<QSharedPointer<League> > Player::leagues() const
{
    return m_leagues;
}

void Player::setLeagues(const QList<QSharedPointer<League> > &arg)
{
    m_leagues = arg;
}

void Player::setOfflineGameInformation(const QList<QSharedPointer<OLD_OfflineGameInformation> > &games)
{
    m_offlineGameInformation = games;
}


QString Player::genderString() const
{
    switch(m_gender) {
    case Male:
        return tr("male");
    case Female:
        return tr("female");
    case UnkownGender:
    default:
        break;
    }
    return tr("");
}

QString Player::name() const
{
    return m_name;
}

void Player::setName(const QString &name)
{
    m_name = name;
}

QList<QSharedPointer<Place> > Player::places() const
{
    return m_places;
}

void Player::setPlaces(const QList<QSharedPointer<Place> > &places)
{
    m_places = places;
}

QList<QSharedPointer<LiveDrink> > Player::liveDrinks() const
{
    return m_liveDrinks;
}

QList<QSharedPointer<Drink> > Player::drinks() const
{
    QMap<QSharedPointer<Drink>, int> counts;

    foreach(QSharedPointer<LiveDrink> drink, liveDrinks()) {
        ++counts[drink->drink()];
    }

    QMultiMap<int, QSharedPointer<Drink> > helperMap;
    for(auto it = counts.constBegin(); it != counts.constEnd(); ++it) {
        helperMap.insert(it.value(), it.key());
    }

    return Qp::reversed(helperMap.values());
}

void Player::setLiveDrinks(const QList<QSharedPointer<LiveDrink> > &drinks)
{
    m_liveDrinks = drinks;
}

QList<QSharedPointer<Game> > Player::games() const
{
    return m_games;
}

int Player::gamePoints() const
{
    return m_allGamesStatistics->gamePoints();
}

int Player::points() const
{
    return m_allGamesStatistics->points();
}

double Player::average() const
{
    return m_allGamesStatistics->average();
}

int Player::wins() const
{
    return m_allGamesStatistics->wins().size();
}

int Player::losses() const
{
    return m_allGamesStatistics->losses().size();
}

double Player::averagePlacement() const
{
    return m_allGamesStatistics->averagePlacement();
}

QSharedPointer<Game> Player::lastGame() const
{
    return m_allGamesStatistics->lastGame();
}

QSharedPointer<Game> Player::lastWin() const
{
    return m_allGamesStatistics->lastWin();
}

void Player::setGames(const QList<QSharedPointer<Game> > &games)
{
    m_games = games;
}

QList<QSharedPointer<Round> > Player::re2Rounds() const
{
    return m_re2Rounds;
}

QList<QSharedPointer<Round> > Player::reRounds() const
{
    if(!m_reRoundsCached.isEmpty())
        return m_reRoundsCached;

    m_reRoundsCached = re1Rounds();
    m_reRoundsCached.append(re2Rounds());
    return m_reRoundsCached;
}

double Player::rePercentage() const
{
    return m_allGamesStatistics->rePercentage();
}

QList<QSharedPointer<Round> > Player::reWins() const
{
    return m_allGamesStatistics->reWins();
}

double Player::reWinsPercentage() const
{
    return m_allGamesStatistics->reWinsPercentage();
}

QList<QSharedPointer<Round> > Player::rounds() const
{
    if(!m_roundsCached.isEmpty())
        return m_roundsCached;

    m_roundsCached = reRounds();
    m_roundsCached.append(contraRounds());
    return m_roundsCached;
}

QList<QSharedPointer<Round> > Player::contraRounds() const
{
    if(!m_contraRoundsCached.isEmpty())
        return m_contraRoundsCached;

    m_contraRoundsCached = contra1Rounds();
    m_contraRoundsCached.append(contra2Rounds());
    m_contraRoundsCached.append(contra3Rounds());
    return m_contraRoundsCached;
}

double Player::contraPercentage() const
{
    return m_allGamesStatistics->contraPercentage();
}

QList<QSharedPointer<Round> > Player::contraWins() const
{
    return m_allGamesStatistics->contraWins();
}

double Player::contraWinsPercentage() const
{
    return m_allGamesStatistics->contraWinsPercentage();
}

double Player::averagePointsPerRound() const
{
    return m_allGamesStatistics->averagePointsPerRound();
}

QList<QSharedPointer<Round> > Player::winRounds() const
{
    return m_allGamesStatistics->winRounds();
}

double Player::roundWinsPercentage() const
{
    return m_allGamesStatistics->roundWinsPercentage();
}

void Player::setRe2Rounds(const QList<QSharedPointer<Round> > &re2Rounds)
{
    m_reRoundsCached.clear();
    m_roundsCached.clear();
    m_re2Rounds = re2Rounds;
}

QList<QSharedPointer<Round> > Player::re1Rounds() const
{
    return m_re1Rounds;
}

void Player::setRe1Rounds(const QList<QSharedPointer<Round> > &re1Rounds)
{
    m_reRoundsCached.clear();
    m_roundsCached.clear();
    m_re1Rounds = re1Rounds;
}

QList<QSharedPointer<Round> > Player::schweinereiRounds() const
{
    return m_schweinereiRounds;
}

void Player::setSchweinereiRounds(const QList<QSharedPointer<Round> > &schweinereiRounds)
{
    m_schweinereiRounds = schweinereiRounds;
}

QList<QSharedPointer<Round> > Player::soloRounds() const
{
    return m_soloRounds;
}

void Player::setSoloRounds(const QList<QSharedPointer<Round> > &soloRounds)
{
    m_soloRounds = soloRounds;
}

QList<QSharedPointer<Round> > Player::trumpfabgabeRounds() const
{
    return m_trumpfabgabeRounds;
}

void Player::setTrumpfabgabeRounds(const QList<QSharedPointer<Round> > &trumpfabgabeRounds)
{
    m_trumpfabgabeRounds = trumpfabgabeRounds;
}

QList<QSharedPointer<Round> > Player::hochzeitRounds() const
{
    return m_hochzeitRounds;
}

void Player::setHochzeitRounds(const QList<QSharedPointer<Round> > &hochzeitRounds)
{
    m_hochzeitRounds = hochzeitRounds;
}

QList<QSharedPointer<Schmeisserei> > Player::schmeissereien() const
{
    return m_schmeissereien;
}

QList<QSharedPointer<OLD_OfflineGameInformation> > Player::offlineGameInformation() const
{
    return m_offlineGameInformation;
}

void Player::setSchmeissereien(const QList<QSharedPointer<Schmeisserei> > &schmeissereien)
{
    m_schmeissereien = schmeissereien;
}

QList<QSharedPointer<Round> > Player::contra1Rounds() const
{
    return m_contra1Rounds;
}

QList<QSharedPointer<Round> > Player::contra2Rounds() const
{
    return m_contra2Rounds;
}

QList<QSharedPointer<Round> > Player::contra3Rounds() const
{
    return m_contra3Rounds;
}

void Player::setContra1Rounds(QList<QSharedPointer<Round> > arg)
{
    m_contraRoundsCached.clear();
    m_roundsCached.clear();
    m_contra1Rounds = arg;
}

void Player::setContra2Rounds(QList<QSharedPointer<Round> > arg)
{
    m_contraRoundsCached.clear();
    m_roundsCached.clear();
    m_contra2Rounds = arg;
}

void Player::setContra3Rounds(QList<QSharedPointer<Round> > arg)
{
    m_contraRoundsCached.clear();
    m_roundsCached.clear();
    m_contra3Rounds = arg;
}

void Player::addRe1Round(QSharedPointer<Round> round)
{
    m_reRoundsCached.clear();
    m_re1Rounds.add(round);
}

void Player::addRe2Round(QSharedPointer<Round> round)
{
    m_reRoundsCached.clear();
    m_re2Rounds.add(round);
}

void Player::addContra1Round(QSharedPointer<Round> round)
{
    m_contraRoundsCached.clear();
    m_contra1Rounds.add(round);
}

void Player::addContra2Round(QSharedPointer<Round> round)
{
    m_contraRoundsCached.clear();
    m_contra2Rounds.add(round);
}

void Player::addContra3Round(QSharedPointer<Round> round)
{
    m_contraRoundsCached.clear();
    m_contra3Rounds.add(round);
}

QSharedPointer<PlayerStatistics> Player::allGamesStatistics() const
{
    return m_allGamesStatistics;
}

void Player::addLiveDrink(QSharedPointer<LiveDrink> drink)
{
    m_liveDrinks.add(drink);
}

void Player::removeLiveDrink(QSharedPointer<LiveDrink> drink)
{
    m_liveDrinks.remove(drink);
}
