#include "schmeisserei.h"

#include "player.h"
#include "round.h"



Schmeisserei::Schmeisserei(QObject *parent) :
    ParseObject(parent),
    m_type(UnkownType),
    m_round(QpRelation(&Schmeisserei::round)),
    m_player(QpRelation(&Schmeisserei::player))
{
}

QSharedPointer<Player> Schmeisserei::player() const
{
    return m_player;
}

void Schmeisserei::setPlayer(const QSharedPointer<Player> &player)
{
    m_player = player;
}

QSharedPointer<Round> Schmeisserei::round() const
{
    return m_round;
}

void Schmeisserei::setRound(const QSharedPointer<Round> &round)
{
    m_round = round;
}

Schmeisserei::Type Schmeisserei::type() const
{
    return m_type;
}

void Schmeisserei::setType(const Type &type)
{
    m_type = type;
}

QString Schmeisserei::typeString() const
{
    return typeStringFromType(type());
}

QStringList Schmeisserei::typeStrings()
{
    return QStringList() << tr("5 Könige") <<
                            tr("Trumpfabgabe nicht genommen") <<
                            tr("Weniger als 3 Trumpf") <<
                            tr("Mehr als 90 Punkte") <<
                            tr("Nichts über Fuchs");
}

QString Schmeisserei::typeStringFromType(Schmeisserei::Type type)
{
    int typeIndex = static_cast<int>(type);
    typeIndex -= 1; // UnkownType
    QStringList types = typeStrings();
    if(typeIndex < 0 || typeIndex >= types.size())
        return tr("Unkown solo");

    return types.at(typeIndex);
}

Schmeisserei::Type Schmeisserei::typeFromString(const QString &typeString)
{
    int typeIndex = typeStrings().indexOf(typeString);
    if(typeIndex < 0)
        return Schmeisserei::UnkownType;

    typeIndex += 1; // UnkownType
    return static_cast<Schmeisserei::Type>(typeIndex);

}

int Schmeisserei::kingsCount() const
{
    return m_kingsCount;
}

int Schmeisserei::trumpfCount() const
{
    return m_trumpfabgabeCount;
}

int Schmeisserei::points() const
{
    return m_points;
}

void Schmeisserei::setKingsCount(int arg)
{
    m_kingsCount = arg;
}

void Schmeisserei::setTrumpfCount(int arg)
{
    m_trumpfabgabeCount = arg;
}

void Schmeisserei::setPoints(int arg)
{
    m_points = arg;
}


QByteArray Schmeisserei::JSONData()
{
    QByteArray postData;
    postData.append("{");
    postData.append(QString("\"kingsCount\"") + ":" +  QString::number(kingsCount()));
    postData.append(",");
    postData.append(QString("\"playerID\"") + ":" + "\"" + player()->parseID() + "\"");
    postData.append(",");
    postData.append(QString("\"points\"") + ":" + QString::number(points()));
    postData.append(",");
    postData.append(QString("\"roundID\"") + ":" + "\"" + round()->parseID() + "\"");
    postData.append(",");
    postData.append(QString("\"type\"") + ":" + QString::number(type()));
    postData.append("}");

    return postData;
}
