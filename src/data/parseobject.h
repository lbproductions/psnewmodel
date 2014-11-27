#ifndef PARSEOBJECT_H
#define PARSEOBJECT_H

#include <QObject>
#include <QSharedPointer>
#include <QDateTime>
#include <QJsonObject>

#include "parsecontroller.h"

class QNetworkReply;
class QNetworkAccessManager;

class ParseObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString parseID READ parseID WRITE setParseID)
    Q_PROPERTY(bool parseUpdated READ parseUpdated WRITE setParseUpdated)
    Q_PROPERTY(QDateTime parseLastUpdate READ parseLastUpdate WRITE setParseLastUpdate)

public:
    explicit ParseObject(QObject *parent = 0);

    QString parseID() const;

    bool parseUpdated() const;

    virtual QByteArray parseJSONData() = 0;
    virtual void parseUpdateFromJSON(QJsonObject object, bool created = false) = 0;

    QString parseJSONString(QSharedPointer<ParseObject> _object);
    QString parseJSONString(QList<QSharedPointer<ParseObject> > _objects);

    template<class T>
    QSharedPointer<T> objectFromJSONPointer(QJsonObject object);

    QDateTime parseLastUpdate() const;

    virtual bool parseCheckPreUploadConditions();
    virtual bool parseCheckAfterUploadConditions();

    void parseUpload();

    bool isParseUploading() const;
    void setIsParseUploading(bool isParseUploading);

    void parseUpdate();

    bool isParseFetching() const;
    void setIsParseFetching(bool isParseFetching);

    void emitParseUpdated();

    QJsonObject parseBatchObject();
    virtual QJsonObject parseObject();


signals:
    void parseUploaded();
    void parseUploadFailed(QString error);
    void parseUpdateCompleted();

public slots:
    void setParseID(QString arg);

    void setParseUpdated(bool arg);

    void setParseLastUpdate(QDateTime arg);

    void parseOnUploadFinished(QNetworkReply* reply);

protected:
    QColor colorFromParseString(QString colorString);
    QString colorToParseString(QColor color);

    static QHash<QString, QSharedPointer<ParseObject>> currentUploadingObjects;

    QString m_parseID;
    bool m_parseUpdated;
    QDateTime m_parseLastUpdate;

    bool m_isUploading;
    bool m_isParseFetching;

    QNetworkAccessManager* m_networkManager;
};

template<class T>
QSharedPointer<T> ParseObject::objectFromJSONPointer(QJsonObject object)
{
    QString id = object.value("objectId").toString();
    return ParseController::instance()->objectFromCache<T>(id);
}

#endif // PARSEOBJECT_H
