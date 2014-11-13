#include "place.h"

#include "player.h"
#include "game.h"

#include <QPersistence.h>



Place::Place(QObject *parent) :
    ParseObject(parent),
    m_postalCode(-1),
    m_houseNumber(-1),
    m_players(QpRelation(&Place::players)),
    m_games(QpRelation(&Place::games))
{
}

Place::~Place()
{
}

QPixmap Place::cityEmblem() const
{
    return m_cityEmblem;
}

void Place::setCityEmblem(const QPixmap &icon)
{
    m_cityEmblem = icon;
}

QString Place::comment() const
{
    return m_comment;
}

void Place::setComment(const QString &comment)
{
    m_comment = comment;
}

int Place::houseNumber() const
{
    return m_houseNumber;
}

void Place::setHouseNumber(int number)
{
    m_houseNumber = number;
}

QString Place::street() const
{
    return m_street;
}

void Place::setStreet(const QString &street)
{
    m_street = street;
}

QString Place::city() const
{
    return m_city;
}

void Place::setCity(const QString &city)
{
    m_city = city;
}

int Place::postalCode() const
{
    return m_postalCode;
}

void Place::setPostalCode(int postalCode)
{
    m_postalCode = postalCode;
}

QList<QSharedPointer<Player> > Place::players() const
{
    return m_players;
}

QList<QSharedPointer<Game> > Place::games() const
{
    return m_games;
}

void Place::setGames(const QList<QSharedPointer<Game> > &games)
{
    m_games = games;
}


QString Place::displayString() const
{
    return QString("%1 %2, %3")
            .arg(m_street)
            .arg(m_houseNumber)
            .arg(m_city);
}

void Place::addPlayer(QSharedPointer<Player> player)
{
    m_players.add(player);
}

void Place::removePlayer(QSharedPointer<Player> player)
{
    m_players.remove(player);
}

void Place::addGame(QSharedPointer<Game> game)
{
    m_games.add(game);
}

void Place::removeGame(QSharedPointer<Game> game)
{
    m_games.remove(game);
}

void Place::setPlayers(const QList<QSharedPointer<Player> > &players)
{
    m_players = players;
}

QByteArray Place::JSONData()
{
    QByteArray postData;
    postData.append("{");
    postData.append(QString("\"city\"") + ":" + "\"" + city() + "\"");
    postData.append(",");
    postData.append(QString("\"houseNumber\"") + ":" + "\"" + QString::number(houseNumber()) + "\"");
    postData.append(",");
    postData.append(QString("\"postalCode\"") + ":" + "\"" + QString::number(postalCode()) + "\"");
    postData.append(",");
    postData.append(QString("\"street\"") + ":" + "\"" + street() + "\"");
    postData.append("}");

    return postData;
}
