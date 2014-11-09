#include "parseobject.h"

ParseObject::ParseObject(QObject *parent) :
    QObject(parent)
{
}

QString ParseObject::parseID() const
{
    return m_parseID;
}

void ParseObject::setParseID(QString arg)
{
    m_parseID = arg;
}
