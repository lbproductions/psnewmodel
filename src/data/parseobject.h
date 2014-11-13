#ifndef PARSEOBJECT_H
#define PARSEOBJECT_H

#include <QObject>
#include <QSharedPointer>

class ParseObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString parseID READ parseID WRITE setParseID)
    Q_PROPERTY(bool parseUpdated READ parseUpdated WRITE setParseUpdated)

public:
    explicit ParseObject(QObject *parent = 0);

    QString parseID() const;

    bool parseUpdated() const;

    virtual QByteArray JSONData() = 0;

    QString JSONString(QSharedPointer<ParseObject> _object);
    QString JSONString(QList<QSharedPointer<ParseObject> > _objects);

signals:

public slots:
    void setParseID(QString arg);

    void setParseUpdated(bool arg);

private:
    QString m_parseID;
    bool m_parseUpdated;
};

#endif // PARSEOBJECT_H
