#include "parsecontroller.h"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "game.h"
#include "round.h"
#include "player.h"
#include "place.h"
#include "schmeisserei.h"
#include "livedrink.h"
#include "drink.h"

ParseController::ParseController(QObject *parent) :
    QObject(parent),
    m_isUpdating(false),
    m_isUploading(false)
{
    createCaches();
}

ParseController *ParseController::instance()
{
    static ParseController controller;
    return &controller;
}

void ParseController::syncData()
{
    auto conn = std::make_shared<QMetaObject::Connection>();
    *conn = connect(this, &ParseController::updateFinished, [=] {
        QObject::disconnect(*conn);

    });
    update();
}

void ParseController::createCaches()
{
    registerCacheClass<Game>();
    registerCacheClass<Round>();
    registerCacheClass<Player>();
    registerCacheClass<Place>();
    registerCacheClass<Drink>();
    registerCacheClass<LiveDrink>();
    registerCacheClass<Schmeisserei>();
}

void ParseController::addToCache(QSharedPointer<ParseObject> object)
{
    QHash<QString, QSharedPointer<ParseObject>> hash = m_cache.value(object->metaObject()->className());
    hash.insert(object->parseID(), object);
    m_cache.insert(object->metaObject()->className(), hash);
}

void ParseController::uploadGames()
{
    QJsonArray array;
    QList<QSharedPointer<Player>> players = Qp::readAll<Player>();
    foreach(QSharedPointer<Player> player, players) {
        array.append(player->parseBatchObject());
    }
    QJsonObject requestObject;
    requestObject.insert("requests", QJsonValue(array));

    QJsonDocument doc(requestObject);
    qDebug() << doc.toJson();

    QNetworkAccessManager* mgr = new QNetworkAccessManager(this);
    QObject::connect(mgr, &QNetworkAccessManager::finished, [=] (QNetworkReply* reply){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray array = document.array();
        for(int i = 0; i<array.size(); i++) {
            QSharedPointer<Player> player = players.at(i);
            if(player->name() == "Patrick" || player->name() == "Niclas") {
                continue;
            }
            QString parseID = array.at(i).toObject().value("success").toObject().value("objectId").toString();
            if(parseID != "") {
                player->setParseID(parseID);
                player->setParseUpdated(true);
                player->setParseLastUpdate(QDateTime::currentDateTime());
                Qp::update(player);
            }
        }

        reply->deleteLater();
        mgr->deleteLater();
    });

    QUrl url("https://api.parse.com/1/batch");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("X-Parse-Application-Id", "rmamC3zOAKDUorbjF6Lb75W4hJFcG97M4myz2u6v");
    request.setRawHeader("X-Parse-REST-API-Key", "KnX9CpfpKk58yQfPp2z0vew1XzayL4E8Swt1ga2z");

    mgr->post(request, doc.toJson());

    return;

    /*
    int count = 0;

    QList<QSharedPointer<Game>> games = Qp::readAll<Game>();
    foreach(QSharedPointer<Game> game, games) {
        if((game->parseID() == "" || !game->parseUpdated()) && game->type() == Game::Doppelkopf && Qp::primaryKey(game) != 271) {
            if(game->rounds().size() > 0 && count < 1) {
                uploadGame(game);
                count++;
            }
        }
    }
    */
}

void ParseController::uploadGame(QSharedPointer<Game> game)
{
    if(!m_uploadingGames.contains(game)) {
        m_uploadingGames.append(game);
    }
    else{
        return;
    }

    if(m_isUpdating) {
        connect(this, &ParseController::updateFinished, this, &ParseController::uploadNextGame);
        return;
    }

    if(!m_isUploading) {
        m_isUploading = true;
        uploadNextGame();
    }
}

void ParseController::update()
{
    if(m_isUpdating) {
        return;
    }

    if(m_isUploading) {
        return;
    }

    m_isUpdating = true;
    m_updateStartTime = QDateTime::currentDateTime();

    _update();
}

void ParseController::uploadNextGame()
{
    if(m_uploadingGames.isEmpty()) {
        m_isUploading = false;
        emit gamesUploadFinished();
        return;
    }

    QSharedPointer<Game> gameToUpload = m_uploadingGames.takeFirst();
    connect(gameToUpload.data(), &Game::parseUploaded, this, &ParseController::uploadNextGame);
    connect(gameToUpload.data(), &Game::parseUploadFailed, this, &ParseController::uploadNextGame);
    gameToUpload->parseUpload();
}

void ParseController::_update()
{
    if(!m_updatedClass.contains("Player")) {
        //updateClass<Player>();
        //return;
    }
    if(!m_updatedClass.contains("Drink")) {
        //updateClass<Drink>();
        //return;
    }
    if(!m_updatedClass.contains("Place")) {
        updateClass<Place>();
        return;
    }
    if(!m_updatedClass.contains("Game")) {
        updateClass<Game>();
        return;
    }
    if(!m_updatedClass.contains("Round")) {
        updateClass<Round>();
        return;
    }
    if(!m_updatedClass.contains("LiveDrink")) {
        updateClass<LiveDrink>();
        return;
    }
    if(!m_updatedClass.contains("Schmeisserei")) {
        updateClass<Schmeisserei>();
        return;
    }

    m_updatedClass.clear();
    m_isUpdating = false;

    emit updateFinished();
}

template<class T>
void ParseController::updateClass()
{
    QString className = T::staticMetaObject.className();

    QDateTime lastUpdate = QDateTime(QDate(2000,1,1));
    if(QSettings().value("lastParseUpdate/"+className).isValid()) {
        //lastUpdate = QSettings().value("lastParseUpdate/"+className).toDateTime();
    }

    QNetworkAccessManager* mgr = new QNetworkAccessManager(this);
    QObject::connect(mgr, &QNetworkAccessManager::finished, [=] (QNetworkReply* reply){
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray replyArray = reply->readAll();

            QJsonDocument document = QJsonDocument::fromJson(replyArray);
            QJsonArray jsonObjects = document.object().value("results").toArray();
            for(int i = 0; i<jsonObjects.size(); i++) {
                QJsonObject jsonObject = jsonObjects.at(i).toObject();
                QJsonValue value = jsonObject.value("objectId");
                QString id = value.toString();

                QSharedPointer<ParseObject> parseObject = m_cache.value(className).value(id);
                if(!parseObject) {
                    parseObject = Qp::create<T>();
                    parseObject->setParseID(id);
                    parseObject->setParseUpdated(true);
                    parseObject->setParseLastUpdate(QDateTime::currentDateTime());
                    parseObject->setParseCreated(QDateTime::fromString(jsonObject.value("createdAt").toString(), Qt::ISODate));
                    parseObject->parseUpdateFromJSON(jsonObject, true);
                    Qp::update(parseObject);
                }
                else {
                    QDateTime updateTime = QDateTime::fromString(jsonObject.value("updatedAt").toString(), Qt::ISODate);
                    QDateTime parseLastUpdate = parseObject->parseLastUpdate();
                    if(updateTime > parseLastUpdate) {
                        parseObject->parseUpdateFromJSON(jsonObject);
                        Qp::update(parseObject);
                    }
                }
                addToCache(parseObject);
                parseObject->emitParseUpdated();
            }

            QSettings().setValue("lastParseUpdate/"+className, QDateTime::currentDateTime());
            m_updatedClass.append(className);
            _update();
        }
        else {
            qDebug() << "Failure updating class " << className << ": " <<reply->errorString();
            m_isUpdating = false;
            m_updatedClass.clear();
        }

        reply->deleteLater();
        mgr->deleteLater();
    });

    QUrl url("https://api.parse.com/1/classes/"+className);

    QUrlQuery query;
    query.addQueryItem("where={\"updatedAt\":{\"$gte\": {\"__type\":\"Date\", \"iso\":\""+ lastUpdate.toString(Qt::ISODate)
                       +"\"}, \"$lte\": {\"__type\":\"Date\", \"iso\":\""+ m_updateStartTime.toString(Qt::ISODate) +"\"}}}", "");
    query.addQueryItem("limit","1000");
    query.addQueryItem("order", "createdAt");

    QString queryString = query.query();
    //queryString.remove(queryString.size()-1, 1);
    qDebug() << queryString;
    //url.setQuery("where={\"finishedRoundsCount\":{\"$gte\":20}}");
    url.setQuery(queryString);
    qDebug() << url;

    QNetworkRequest request(url);
    request.setRawHeader("X-Parse-Application-Id", "rmamC3zOAKDUorbjF6Lb75W4hJFcG97M4myz2u6v");
    request.setRawHeader("X-Parse-REST-API-Key", "KnX9CpfpKk58yQfPp2z0vew1XzayL4E8Swt1ga2z");

    mgr->get(request);
}

template<class T>
void ParseController::registerCacheClass()
{
    QHash<QString, QSharedPointer<ParseObject>> hash;

    QList<QSharedPointer<T>> list = Qp::readAll<T>();
    if(list.isEmpty()) {
        return;
    }

    foreach(QSharedPointer<T> object, list) {
        if(object->parseID() != "") {
            hash.insert(object->parseID(), object);
        }
    }

    m_cache.insert(list.first()->metaObject()->className(), hash);
}


bool sortParseObjectsByCreationTime(QSharedPointer<ParseObject> obj1, QSharedPointer<ParseObject> obj2)
{
    return obj1->parseCreated() < obj2->parseCreated();
}
