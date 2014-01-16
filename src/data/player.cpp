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
    return m_leagues.resolveList();
}

void Player::setLeagues(const QList<QSharedPointer<League> > &arg)
{
    m_leagues.relate(arg);
}

void Player::setOfflineGameInformation(const QList<QSharedPointer<OLD_OfflineGameInformation> > &games)
{
    m_offlineGameInformation.relate(games);
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
    return m_places.resolveList();
}

void Player::setPlaces(const QList<QSharedPointer<Place> > &places)
{
    m_places.clear();
    m_places.relate(places);
}

QList<QSharedPointer<LiveDrink> > Player::liveDrinks() const
{
    return m_liveDrinks.resolveList();
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
    m_liveDrinks.relate(drinks);
}

QList<QSharedPointer<Game> > Player::games() const
{
    return m_games.resolveList();
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
    m_games.clear();
    m_games.relate(games);
}

QList<QSharedPointer<Round> > Player::re2Rounds() const
{
    return m_re2Rounds.resolveList();
}

QList<QSharedPointer<Round> > Player::reRounds() const
{
    QList<QSharedPointer<Round> > result = re1Rounds();
    result.append(re2Rounds());
    return result;
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
    QList<QSharedPointer<Round> > result;
    result.append(reRounds());
    result.append(contraRounds());
    return result;
}

QList<QSharedPointer<Round> > Player::contraRounds() const
{
    QList<QSharedPointer<Round> > result = contra1Rounds();
    result.append(contra2Rounds());
    result.append(contra3Rounds());
    return result;
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
    m_re2Rounds.relate(re2Rounds);
}

QList<QSharedPointer<Round> > Player::re1Rounds() const
{
    return m_re1Rounds.resolveList();
}

void Player::setRe1Rounds(const QList<QSharedPointer<Round> > &re1Rounds)
{
    m_re1Rounds.relate(re1Rounds);
}

QList<QSharedPointer<Round> > Player::schweinereiRounds() const
{
    return m_schweinereiRounds.resolveList();
}

void Player::setSchweinereiRounds(const QList<QSharedPointer<Round> > &schweinereiRounds)
{
    m_schweinereiRounds.relate(schweinereiRounds);
}

QList<QSharedPointer<Round> > Player::soloRounds() const
{
    return m_soloRounds.resolveList();
}

void Player::setSoloRounds(const QList<QSharedPointer<Round> > &soloRounds)
{
    m_soloRounds.relate(soloRounds);
}

QList<QSharedPointer<Round> > Player::trumpfabgabeRounds() const
{
    return m_trumpfabgabeRounds.resolveList();
}

void Player::setTrumpfabgabeRounds(const QList<QSharedPointer<Round> > &trumpfabgabeRounds)
{
    m_trumpfabgabeRounds.relate(trumpfabgabeRounds);
}

QList<QSharedPointer<Round> > Player::hochzeitRounds() const
{
    return m_hochzeitRounds.resolveList();
}

void Player::setHochzeitRounds(const QList<QSharedPointer<Round> > &hochzeitRounds)
{
    m_hochzeitRounds.relate(hochzeitRounds);
}

QList<QSharedPointer<Schmeisserei> > Player::schmeissereien() const
{
    return m_schmeissereien.resolveList();
}

QList<QSharedPointer<OLD_OfflineGameInformation> > Player::offlineGameInformation() const
{
    return m_offlineGameInformation.resolveList();
}

void Player::setSchmeissereien(const QList<QSharedPointer<Schmeisserei> > &schmeissereien)
{
    m_schmeissereien.relate(schmeissereien);
}

QList<QSharedPointer<Round> > Player::contra1Rounds() const
{
    return m_contra1Rounds.resolveList();
}

QList<QSharedPointer<Round> > Player::contra2Rounds() const
{
    return m_contra2Rounds.resolveList();
}

QList<QSharedPointer<Round> > Player::contra3Rounds() const
{
    return m_contra3Rounds.resolveList();
}

void Player::setContra1Rounds(QList<QSharedPointer<Round> > arg)
{
    m_contra1Rounds.relate(arg);
}

void Player::setContra2Rounds(QList<QSharedPointer<Round> > arg)
{
    m_contra2Rounds.relate(arg);
}

void Player::setContra3Rounds(QList<QSharedPointer<Round> > arg)
{
    m_contra3Rounds.relate(arg);
}

QSharedPointer<PlayerStatistics> Player::allGamesStatistics() const
{
    return m_allGamesStatistics;
}
