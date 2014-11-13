#include "parseobject.h"

ParseObject::ParseObject(QObject *parent) :
    QObject(parent),
    m_parseUpdated(false)
{
}

QString ParseObject::JSONString(QSharedPointer<ParseObject> _object)
{
    return QString("{")
            + "\"__type\":\"Pointer\","
            + "\"className\":\""+ _object->metaObject()->className() + "\","
            + "\"objectId\":\""
            + _object->parseID()
            + "\""
            + "}";
}

QString ParseObject::JSONString(QList<QSharedPointer<ParseObject> > _objects)
{
    QString value = "{\"__op\":\"AddUnique\",\"objects\":[";
    foreach(QSharedPointer<ParseObject> object, _objects) {
        value += JSONString(object) + ",";
    }
    value.remove(value.size()-1, 1);
    value += "]}";

    return value;
}

QString ParseObject::parseID() const
{
    return m_parseID;
}

void ParseObject::setParseID(QString arg)
{
    m_parseID = arg;
}

bool ParseObject::parseUpdated() const
{
    return m_parseUpdated;
}

void ParseObject::setParseUpdated(bool arg)
{
    m_parseUpdated = arg;
}
