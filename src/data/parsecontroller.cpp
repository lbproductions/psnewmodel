#include "parsecontroller.h"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "game.h"
#include "round.h"
#include "player.h"
#include "place.h"

static QString applicationKey = "rmamC3zOAKDUorbjF6Lb75W4hJFcG97M4myz2u6v";
static QString restKey = "KnX9CpfpKk58yQfPp2z0vew1XzayL4E8Swt1ga2z";

ParseController::ParseController(QObject *parent) :
    QObject(parent),
    m_uploadingGame(false)
{
    m_gameNetworkManager = new QNetworkAccessManager(this);
    m_roundNetworkManager = new QNetworkAccessManager(this);
    m_placeNetworkManager = new QNetworkAccessManager(this);
    m_playerNetworkManager = new QNetworkAccessManager(this);
}

ParseController *ParseController::instance()
{
    static ParseController controller;
    return &controller;
}

void ParseController::uploadGames()
{
    QList<QSharedPointer<Game>> games = Qp::readAll<Game>();
    /*
    foreach(QSharedPointer<Game> game, games) {
        if(!m_gamesToUpload.contains(game)) {
            m_gamesToUpload.append(game);
        }
    }
    */

    tryToUploadGame(games.last());
}

void ParseController::tryToUploadGame(QSharedPointer<Game> _game)
{
    if(!m_gamesToUpload.contains(_game)) {
        m_gamesToUpload.append(_game);
    }

    if(!m_uploadingGame) {
        m_uploadingGame = true;
        uploadGame(_game);
    }
}

void ParseController::uploadGame(QSharedPointer<Game> _game)
{
    if(!checkGameDependendUploads(_game)) {
        return;
    }

    connect(m_gameNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onGameUploadFinished(QNetworkReply*)));

    QString urlString = "https://api.parse.com/1/classes/Game";
    QUrl url(urlString);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("X-Parse-Application-Id", applicationKey.toUtf8());
    request.setRawHeader("X-Parse-REST-API-Key", restKey.toUtf8());

    QByteArray postData;
    if(_game->mitPflichtSolo()) {
        postData.append("{\"pflichtsolo\":true,");
    }
    else {
        postData.append("{\"pflichtsolo\":false,");
    }
    postData.append("\"state\":"+QString::number(_game->state())+",");
    postData.append("\"type\":"+QString::number(_game->type())+",");
    postData.append("\"finishedRoundsCount\":"+QString::number(_game->finishedRoundCount()));
    QString playersString = "\"players\":{\"__op\":\"Add\",\"objects\":[";
    foreach(QSharedPointer<Player> player, _game->players()) {
        playersString += QString("{")
                + "\"__type\":\"Pointer\","
                + "\"className\":\"Player\","
                + "\"objectId\":\""
                + player->parseID()
                + "\"";
                + "}";
    }

    postData.append(playersString);
    postData.append("}");

    m_gameNetworkManager->post(request, postData);
}

void ParseController::onGameUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QSharedPointer<Game> uploadedGame = m_gamesToUpload.takeFirst();

        QByteArray replyArray = reply->readAll();
        qDebug() << replyArray;
        QJsonDocument document = QJsonDocument::fromJson(replyArray);
        QJsonObject object = document.object();
        qDebug() << object.keys();
        QJsonValue value = object.value("objectId");

        uploadedGame->setParseID(value.toString());
        Qp::update(uploadedGame);

        delete reply;

        if(!m_gamesToUpload.isEmpty()) {
            uploadGame(m_gamesToUpload.first());
        }
        else {
            m_uploadingGame = false;
        }
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
        m_uploadingGame = false;
        delete reply;
    }
}

bool ParseController::checkGameDependendUploads(QSharedPointer<Game> _game)
{
    bool check = true;

    foreach(QSharedPointer<Player> player, _game->players()) {
        if(player->parseID() == "") {
            tryToUploadPlayer(player);
            check = false;
        }
    }

    if(!check) { // Rounds needs uploaded players
        return check;
    }

    foreach(QSharedPointer<Round> round, _game->rounds()) {
        if(round->parseID() == "") {
            tryToUploadRound(round);
            check = false;
        }
    }

    if(_game->site()->parseID() == "") {
        tryToUploadPlace(_game->site());
        check = false;
    }

    return check;
}

void ParseController::tryToUploadRound(QSharedPointer<Round> _round)
{
    if(!m_roundsToUpload.contains(_round)) {
        m_roundsToUpload.append(_round);
    }

    if(!m_uploadingRound) {
        m_uploadingRound = true;
        uploadRound(_round);
    }
}

void ParseController::uploadRound(QSharedPointer<Round> _round)
{
    connect(m_roundNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRoundUploadFinished(QNetworkReply*)));

    QUrl url("https://api.parse.com/1/classes/Round");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("X-Parse-Application-Id", applicationKey.toUtf8());
    request.setRawHeader("X-Parse-REST-API-Key", restKey.toUtf8());

    QByteArray postData;
    postData.append("{");
    postData.append(QString("\"gameID\"") + ":" + "\"" + _round->game()->parseID() + "\"");
    postData.append(",");
    postData.append(QString("\"number\"") + ":" + QString::number(_round->number()));
    postData.append(",");
    postData.append(QString("\"reGamePoints\"") + ":" + QString::number(_round->reGamePoints()));
    postData.append(",");
    postData.append(QString("\"contraGamePoints\"") + ":" + QString::number(_round->contraGamePoints()));
    postData.append(",");
    postData.append(QString("\"state\"") + ":" + QString::number(_round->state()));
    postData.append(",");
    postData.append(QString("\"winnerParty\"") + ":" + QString::number(_round->winnerParty()));
    postData.append(",");
    postData.append(QString("\"hochzeitDecision\"") + ":" + QString::number(_round->hochzeitDecision()));
    postData.append(",");
    if(_round->isPflicht()) {
        postData.append(QString("\"isPflicht\"") + ":" + "true");
    }
    else {
        postData.append(QString("\"isPflicht\"") + ":" + "false");
    }
    postData.append(",");
    postData.append(QString("\"trumpfCount\"") + ":" + QString::number(_round->trumpfCount()));
    postData.append(",");
    postData.append(QString("\"trumpfZurueck\"") + ":" + QString::number(_round->trumpfZurueck()));
    postData.append("}");

    m_roundNetworkManager->post(request, postData);
}

void ParseController::onRoundUploadFinished(QNetworkReply *reply)
{
    if (reply != 0 && reply->error() == QNetworkReply::NoError) {
        QSharedPointer<Round> uploadedRound = m_roundsToUpload.takeFirst();
        qDebug() << "Uploaded round " << uploadedRound->number() << " of game: " << uploadedRound->game()->creationTime().toString();

        QByteArray replyArray = reply->readAll();
        qDebug() << replyArray;
        QJsonDocument document = QJsonDocument::fromJson(replyArray);
        QJsonObject object = document.object();
        qDebug() << object.keys();
        QJsonValue value = object.value("objectId");

        uploadedRound->setParseID(value.toString());
        Qp::update(uploadedRound);

        if(!m_roundsToUpload.isEmpty()) {
            uploadRound(m_roundsToUpload.first());
        }
        else {
            m_uploadingRound = false;

            if(m_gamesToUpload.first() == uploadedRound->game()) {
                uploadGame(uploadedRound->game());
            }
        }
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
        m_uploadingRound = false;
    }
}

void ParseController::onPlaceUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QSharedPointer<Place> uploadedPlace = m_placesToUpload.takeFirst();
        qDebug() << "Uploaded place: " << uploadedPlace->street();

        QByteArray replyArray = reply->readAll();
        qDebug() << replyArray;
        QJsonDocument document = QJsonDocument::fromJson(replyArray);
        QJsonObject object = document.object();
        qDebug() << object.keys();
        QJsonValue value = object.value("objectId");

        uploadedPlace->setParseID(value.toString());
        Qp::update(uploadedPlace);

        delete reply;

        if(!m_placesToUpload.isEmpty()) {
            uploadPlace(m_placesToUpload.first());
        }
        else {
            m_uploadingPlace = false;
        }

        if(m_uploadingGame) {
            uploadGame(m_gamesToUpload.first());
        }
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
        m_uploadingPlace = false;
        delete reply;
    }
}

void ParseController::tryToUploadPlace(QSharedPointer<Place> _place)
{
    if(!m_placesToUpload.contains(_place)) {
        m_placesToUpload.append(_place);
    }

    if(!m_uploadingPlace) {
        m_uploadingPlace = true;
        uploadPlace(_place);
    }
}

void ParseController::uploadPlace(QSharedPointer<Place> _place)
{
    connect(m_placeNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onPlaceUploadFinished(QNetworkReply*)));

    QString urlString = "https://api.parse.com/1/classes/Place";
    QUrl url(urlString);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("X-Parse-Application-Id", applicationKey.toUtf8());
    request.setRawHeader("X-Parse-REST-API-Key", restKey.toUtf8());

    QByteArray postData;
    postData.append("{");
    postData.append(QString("\"city\"") + ":" + "\"" + _place->city() + "\"");
    postData.append(",");
    postData.append(QString("\"houseNumber\"") + ":" + "\"" + QString::number(_place->houseNumber()) + "\"");
    postData.append(",");
    postData.append(QString("\"postalCode\"") + ":" + "\"" + QString::number(_place->postalCode()) + "\"");
    postData.append(",");
    postData.append(QString("\"street\"") + ":" + "\"" + _place->street() + "\"");
    postData.append("}");

    m_placeNetworkManager->post(request, postData);
}

void ParseController::tryToUploadPlayer(QSharedPointer<Player> _player)
{
    if(!m_playersToUpload.contains(_player)) {
        m_playersToUpload.append(_player);
    }

    if(!m_uploadingPlayer) {
        m_uploadingPlayer = true;
        uploadPlayer(_player);
    }
}

void ParseController::uploadPlayer(QSharedPointer<Player> _player)
{
    connect(m_playerNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onPlayerUploadFinished(QNetworkReply*)));

    qDebug() << "Uploading player: " << _player->name();

    QString urlString = "https://api.parse.com/1/classes/Player";
    QUrl url(urlString);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("X-Parse-Application-Id", applicationKey.toUtf8());
    request.setRawHeader("X-Parse-REST-API-Key", restKey.toUtf8());
    QByteArray postData;
    postData.append("{");
    postData.append(QString("\"displayName\"") + ":" + "\"" + _player->name() + "\"");
    postData.append("}");

    m_playerNetworkManager->post(request, postData);
}

void ParseController::onPlayerUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QSharedPointer<Player> uploadedPlayer = m_playersToUpload.takeFirst();
        qDebug() << "Uploaded player: " << uploadedPlayer->name();

        QByteArray replyArray = reply->readAll();
        qDebug() << replyArray;
        QJsonDocument document = QJsonDocument::fromJson(replyArray);
        QJsonObject object = document.object();
        qDebug() << object.keys();
        QJsonValue value = object.value("objectId");

        uploadedPlayer->setParseID(value.toString());
        Qp::update(uploadedPlayer);

        if(!m_playersToUpload.isEmpty()) {
            uploadPlayer(m_playersToUpload.first());
        }
        else {
            m_uploadingPlayer = false;
        }

        if(m_uploadingGame) {
            uploadGame(m_gamesToUpload.first());
        }
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
        m_uploadingPlayer = false;
    }
}


