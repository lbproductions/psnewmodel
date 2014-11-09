#ifndef PARSEOBJECT_H
#define PARSEOBJECT_H

#include <QObject>

class ParseObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString parseID READ parseID WRITE setParseID)

public:
    explicit ParseObject(QObject *parent = 0);

    QString parseID() const;

signals:

public slots:
    void setParseID(QString arg);

private:
    QString m_parseID;
};

#endif // PARSEOBJECT_H
