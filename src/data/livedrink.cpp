#include "livedrink.h"

#include "player.h"
#include "round.h"
#include "drink.h"



LiveDrink::LiveDrink(QObject *parent) :
    ParseObject(parent),
    m_time(QDateTime::currentDateTime()),
    m_player(QpRelation(&LiveDrink::player)),
    m_round(QpRelation(&LiveDrink::round)),
    m_drink(QpRelation(&LiveDrink::drink))
{
}

LiveDrink::~LiveDrink()
{
}

QSharedPointer<Player> LiveDrink::player() const
{
    return m_player;
}

void LiveDrink::setPlayer(QSharedPointer<Player> player)
{
    m_player = player;
}

QSharedPointer<Round> LiveDrink::round() const
{
    return m_round;
}

void LiveDrink::setRound(QSharedPointer<Round> round)
{
    m_round = round;
}

QSharedPointer<Drink> LiveDrink::drink() const
{
    return m_drink;
}

void LiveDrink::setDrink(QSharedPointer<Drink> drink)
{
    m_drink = drink;
}

void LiveDrink::clearRelations()
{
    setRound(QSharedPointer<Round>());
    setPlayer(QSharedPointer<Player>());
    setDrink(QSharedPointer<Drink>());
}

QDateTime LiveDrink::time() const
{
    return m_time;
}

void LiveDrink::setTime(const QDateTime &time)
{
    m_time = time;
}

QByteArray LiveDrink::parseJSONData()
{
    QByteArray postData;
    postData.append("{");
    postData.append(QString("\"drinkID\"") + ":" + "\"" + drink()->parseID() + "\"");
    postData.append(",");
    postData.append(QString("\"playerID\"") + ":" + "\"" + player()->parseID() + "\"");
    postData.append(",");
    postData.append(QString("\"roundID\"") + ":" + "\"" + round()->parseID() + "\"");
    postData.append(",");
    postData.append(QString("\"time\"") + ":" + "{\"__type\": \"Date\",\"iso\": \"" + time().toString(Qt::ISODate) + "\"}");
    postData.append("}");

    return postData;
}

void LiveDrink::parseUpdateFromJSON(QJsonObject object, bool created)
{
    QString dateString = object.value("time").toObject().value("iso").toString();
    setTime(QDateTime::fromString(dateString, Qt::ISODate));

    QSharedPointer<Player> player = ParseController::instance()->objectFromCache<Player>(object.value("playerID").toString());
    Q_ASSERT(player);
    setPlayer(player);

    QSharedPointer<Drink> drink = ParseController::instance()->objectFromCache<Drink>(object.value("drinkID").toString());
    Q_ASSERT(drink);
    setDrink(drink);

    QSharedPointer<Round> round = ParseController::instance()->objectFromCache<Round>(object.value("roundID").toString());
    Q_ASSERT(round);
    setRound(round);

    if(created) {
        round->addLiveDrink(Qp::sharedFrom(this));
    }

    Qp::update(Qp::sharedFrom(this));
    Qp::update(round);

    round->game()->emitParseUpdated();
}


bool LiveDrink::parseCheckPreUploadConditions()
{
    Q_ASSERT(player()->parseID() != "");
    Q_ASSERT(round()->parseID() != "");

    if(drink()->parseID() == "") {
        drink()->parseUpload();
        return false;
    }

    return true;
}


bool LiveDrink::parseCheckAfterUploadConditions()
{
    m_isUploading = false;
    ParseObject::currentUploadingObjects.remove("LiveDrink");

    if(round()->isParseUploading() && round()->parseCheckAfterUploadConditions()) {
        round()->parseUpload();
    }

    return true;
}
