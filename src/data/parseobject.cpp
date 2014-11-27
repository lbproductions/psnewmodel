#include "parseobject.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QColor>

#include <QPersistence.h>

static QString applicationKey = "rmamC3zOAKDUorbjF6Lb75W4hJFcG97M4myz2u6v";
static QString restKey = "KnX9CpfpKk58yQfPp2z0vew1XzayL4E8Swt1ga2z";

QHash<QString, QSharedPointer<ParseObject>> ParseObject::currentUploadingObjects = QHash<QString, QSharedPointer<ParseObject>>();

ParseObject::ParseObject(QObject *parent) :
    QObject(parent),
    m_parseUpdated(false),
    m_parseLastUpdate(QDateTime(QDate(1970,1,1))),
    m_isUploading(false),
    m_networkManager(0)
{}

QString ParseObject::parseJSONString(QSharedPointer<ParseObject> _object)
{
    return QString("{")
            + "\"__type\":\"Pointer\","
            + "\"className\":\""+ _object->metaObject()->className() + "\","
            + "\"objectId\":\""
            + _object->parseID()
            + "\""
            + "}";
}

QString ParseObject::parseJSONString(QList<QSharedPointer<ParseObject> > _objects)
{
    //QString value = "{\"__op\":\"AddUnique\",\"objects\":[";
    QString value = "[";
    foreach(QSharedPointer<ParseObject> object, _objects) {
        value += parseJSONString(object) + ",";
    }
    value.remove(value.size()-1, 1);
    value += "]";

    return value;
}

bool ParseObject::parseCheckPreUploadConditions()
{
    return true;
}

bool ParseObject::parseCheckAfterUploadConditions()
{
    return true;
}

void ParseObject::parseUpload()
{
    if(m_isUploading/* || ParseObject::currentUploadingObjects.value(metaObject()->className()) */) {
        return;
    }

    ParseObject::currentUploadingObjects.insert(metaObject()->className(), Qp::sharedFrom(this));

    if(!parseCheckPreUploadConditions()) {
        return;
    }

    m_isUploading = true;

    if(!m_networkManager) {
        m_networkManager = new QNetworkAccessManager(this);
    }

    connect(m_networkManager, &QNetworkAccessManager::finished, this, &ParseObject::parseOnUploadFinished);

    bool update = false;
    QString urlString = "https://api.parse.com/1/classes/" + QString(metaObject()->className());
    if(parseID() != "") {
        urlString += "/" + parseID();
        update = true;
    }
    qDebug() << "Executing URL: " << urlString;
    QUrl url(urlString);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("X-Parse-Application-Id", applicationKey.toUtf8());
    request.setRawHeader("X-Parse-REST-API-Key", restKey.toUtf8());

    QByteArray postData = parseJSONData();

    if(update) {
        m_networkManager->put(request, postData);
    }
    else {
        m_networkManager->post(request, postData);
    }
}

void ParseObject::parseOnUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray replyArray = reply->readAll();
        if(replyArray.isEmpty()) {
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(replyArray);
        QJsonObject object = document.object();

        if(object.keys().contains("updatedAt")) {
            Q_ASSERT(parseID() != "");
            qDebug() << "Updated " << QString(metaObject()->className()) << " with ID " << parseID();
        }
        else if(object.keys().contains("createdAt")) {
            QJsonValue value = object.value("objectId");
            setParseID(value.toString());
            qDebug() << "Uploaded " << QString(metaObject()->className()) << " with ID " << parseID();
        }

        setParseLastUpdate(QDateTime::currentDateTime());
        Qp::update(Qp::sharedFrom(this));
        ParseController::instance()->addToCache(Qp::sharedFrom(this));

        if(parseCheckAfterUploadConditions()) {
            m_isUploading = false;
            if(ParseObject::currentUploadingObjects.value(metaObject()->className()) == Qp::sharedFrom(this)) {
                ParseObject::currentUploadingObjects.remove(metaObject()->className());
            }

            setParseUpdated(true);
            Qp::update(Qp::sharedFrom(this));
            emit parseUploaded();
        }
        else {
            setParseUpdated(false);
            Qp::update(Qp::sharedFrom(this));
        }
    }
    else {
        qDebug() << "Failure uploading " << metaObject()->className() << ": " << reply->errorString();
        m_isUploading = false;
        emit parseUploadFailed(reply->errorString());
    }

    reply->deleteLater();
    /*
    if(m_networkManager) {
        delete m_networkManager;
        m_networkManager = 0;
    }
    */
}

void ParseObject::parseUpdate()
{
    if(m_isParseFetching) {
        return;
    }

    Q_ASSERT(parseID() != "");

    if(!m_networkManager) {
        m_networkManager = new QNetworkAccessManager(this);
    }

    QObject::connect(m_networkManager, &QNetworkAccessManager::finished, [=] (QNetworkReply* reply){
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray replyArray = reply->readAll();

            QJsonDocument document = QJsonDocument::fromJson(replyArray);
            QJsonObject jsonObject = document.object();
            if(jsonObject.value("updatedAt").toVariant().toDateTime() > parseLastUpdate()) {
                parseUpdateFromJSON(jsonObject);
            }

            setParseLastUpdate(QDateTime::currentDateTime());
            Qp::update(Qp::sharedFrom(this));
        }
        else {
            qDebug() << "Failure updating class " << metaObject()->className() << ": " <<reply->errorString();
        }

        reply->deleteLater();
        m_networkManager->deleteLater();
    });

    QUrl url("https://api.parse.com/1/classes/"+QString(metaObject()->className()) + "/" + parseID());
    qDebug() << url;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("X-Parse-Application-Id", applicationKey.toUtf8());
    request.setRawHeader("X-Parse-REST-API-Key", restKey.toUtf8());

    m_networkManager->get(request);
}

QColor ParseObject::colorFromParseString(QString colorString)
{
    if(colorString == "") {
        return QColor();
    }

    QStringList list = colorString.split(" ");
    Q_ASSERT(list.size() == 4);
    return QColor(list.at(0).toDouble()*255, list.at(1).toDouble()*255, list.at(2).toDouble()*255, list.at(3).toDouble()*255);
}

QString ParseObject::colorToParseString(QColor color)
{
    return QString::number((double)color.red()/(double)255) + " "
            + QString::number((double)color.green()/(double)255) + " "
            + QString::number((double)color.blue()/(double)255) + " "
            + QString::number((double)color.alpha()/(double)255);
}
bool ParseObject::isParseFetching() const
{
    return m_isParseFetching;
}

void ParseObject::setIsParseFetching(bool isParseFetching)
{
    m_isParseFetching = isParseFetching;
}

void ParseObject::emitParseUpdated()
{
    emit parseUpdateCompleted();
}

QJsonObject ParseObject::parseBatchObject()
{
    QJsonObject object;
    object.insert("method", QJsonValue("POST"));
    object.insert("path", QJsonValue("/1/classes/Player"));
    object.insert("body", QJsonValue(parseObject()));
    return object;
}

QJsonObject ParseObject::parseObject()
{
    QJsonObject object;
    return object;
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

void ParseObject::setParseLastUpdate(QDateTime arg)
{
    m_parseLastUpdate = arg;
}
bool ParseObject::isParseUploading() const
{
    return m_isUploading;
}

void ParseObject::setIsParseUploading(bool isUploading)
{
    m_isUploading = isUploading;
}

QDateTime ParseObject::parseLastUpdate() const
{
    return m_parseLastUpdate;
}
