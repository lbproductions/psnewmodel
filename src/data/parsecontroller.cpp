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
#include "schmeisserei.h"
#include "livedrink.h"
#include "drink.h"

static QString applicationKey = "rmamC3zOAKDUorbjF6Lb75W4hJFcG97M4myz2u6v";
static QString restKey = "KnX9CpfpKk58yQfPp2z0vew1XzayL4E8Swt1ga2z";

ParseController::ParseController(QObject *parent) :
    QObject(parent),
    m_uploadingGame(false),
    m_uploadingRound(false),
    m_uploadingPlayer(false),
    m_uploadingPlace(false),
    m_uploadingSchmeisserei(false),
    m_uploadingDrink(false),
    m_uploadingLiveDrink(false)
{
    m_gameNetworkManager = new QNetworkAccessManager(this);
    m_roundNetworkManager = new QNetworkAccessManager(this);
    m_placeNetworkManager = new QNetworkAccessManager(this);
    m_playerNetworkManager = new QNetworkAccessManager(this);
    m_schmeissereiNetworkManager = new QNetworkAccessManager(this);
    m_liveDrinkNetworkManager = new QNetworkAccessManager(this);
    m_drinkNetworkManager = new QNetworkAccessManager(this);
}

ParseController *ParseController::instance()
{
    static ParseController controller;
    return &controller;
}

void ParseController::uploadParseObject(QSharedPointer<ParseObject> _object, QNetworkAccessManager* _networkManager)
{
    bool update = false;
    QString urlString = "https://api.parse.com/1/classes/" + QString(_object->metaObject()->className());
    if(_object->parseID() != "") {
        urlString += "/" + _object->parseID();
        update = true;
    }
    qDebug() << "Executing URL: " << urlString;
    QUrl url(urlString);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("X-Parse-Application-Id", applicationKey.toUtf8());
    request.setRawHeader("X-Parse-REST-API-Key", restKey.toUtf8());

    QByteArray postData = _object->JSONData();

    if(update) {
        _networkManager->put(request, postData);
    }
    else {
        _networkManager->post(request, postData);
    }
}

void ParseController::onParseObjectUploadFinished(QByteArray replyArray, QSharedPointer<ParseObject> _object)
{
    if(replyArray.isEmpty()) {
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(replyArray);
    QJsonObject object = document.object();

    if(object.keys().contains("updatedAt")) {
        Q_ASSERT(_object->parseID() != "");
        qDebug() << "Updated " << QString(_object->metaObject()->className()) << " with ID " << _object->parseID();
        _object->setParseUpdated(true);
        Qp::update(_object);
    }
    else if(object.keys().contains("createdAt")) {
        QJsonValue value = object.value("objectId");
        _object->setParseID(value.toString());
        _object->setParseUpdated(true);
        Qp::update(_object);
        qDebug() << "Uploaded " << QString(_object->metaObject()->className()) << " with ID " << _object->parseID();
    }
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

    uploadParseObject(_game, m_gameNetworkManager);
}

void ParseController::onGameUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        if(m_gamesToUpload.isEmpty()) {
            return;
        }

        QByteArray array = reply->readAll();
        if(array.isEmpty()) {
            return;
        }

        QSharedPointer<Game> uploadedGame = m_gamesToUpload.takeFirst();
        onParseObjectUploadFinished(array, uploadedGame);

        if(checkGameUpdateDependencies(uploadedGame))  {
            if(!m_gamesToUpload.isEmpty()) {
                uploadGame(m_gamesToUpload.first());
            }
            else {
                m_uploadingGame = false;
            }
        }
        else {
            m_gamesToUpload.insert(0, uploadedGame);
            uploadedGame->setParseUpdated(false);
            Qp::update(uploadedGame);
        }
    }
    else {
        qDebug() << "Failure uploading game: " << reply->errorString();
        m_uploadingGame = false;
    }

    reply->deleteLater();
}

bool ParseController::checkGameDependendUploads(QSharedPointer<Game> _game)
{
    bool check = true;

    foreach(QSharedPointer<Player> player, _game->players()) {
        if(player->parseID() == "" /*|| !player->parseUpdated()*/) {
            tryToUploadPlayer(player);
            check = false;
        }
    }

    return check;
}

bool ParseController::checkGameUpdateDependencies(QSharedPointer<Game> _game)
{
    bool check = true;

    foreach(QSharedPointer<Round> round, _game->rounds()) {
        if(round->parseID() == "" || !round->parseUpdated()) {
            tryToUploadRound(round);
            check = false;
        }
    }

    if(_game->site()->parseID() == "" || !_game->site()->parseUpdated()) {
        tryToUploadPlace(_game->site());
        check = false;
    }

    return check;
}

bool ParseController::checkRoundUploadDependencies(QSharedPointer<Round> _round)
{
    if(_round->game()->parseID() == "") {
        tryToUploadGame(_round->game());
        return false;
    }

    return true;
}

bool ParseController::checkRoundUpdateDependencies(QSharedPointer<Round> _round)
{
    bool check = true;

    foreach(QSharedPointer<Schmeisserei> schmeisserei, _round->schmeissereis()) {
        if(schmeisserei->parseID() == "" || !schmeisserei->parseUpdated()) {
            tryToUploadSchmeisserei(schmeisserei);
            check = false;
        }
    }

    foreach(QSharedPointer<LiveDrink> drink, _round->liveDrinks()) {
        if(drink->parseID() == "" || !drink->parseUpdated()) {
            tryToUploadLiveDrink(drink);
            check = false;
        }
    }

    return check;
}

bool ParseController::checkLiveDrinkUploadDependencies(QSharedPointer<LiveDrink> _drink)
{
    bool check = true;

    Q_ASSERT(_drink->player()->parseID() != "");
    Q_ASSERT(_drink->round()->parseID() != "");

    if(_drink->drink()->parseID() == "") {
        tryToUploadDrink(_drink->drink());
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
    if(!checkRoundUploadDependencies(_round)) {
        return;
    }

    connect(m_roundNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRoundUploadFinished(QNetworkReply*)));

    uploadParseObject(_round, m_roundNetworkManager);
}

void ParseController::onRoundUploadFinished(QNetworkReply *reply)
{
    if (reply != 0 && reply->error() == QNetworkReply::NoError) {
        QByteArray replyArray = reply->readAll();
        if(replyArray.isEmpty()) {
            return;
        }
        if(m_roundsToUpload.isEmpty()) {
            return;
        }

        QSharedPointer<Round> uploadedRound = m_roundsToUpload.takeFirst();
        onParseObjectUploadFinished(replyArray, uploadedRound);

        if(checkRoundUpdateDependencies(uploadedRound)) {
            if(m_uploadingGame && checkGameUpdateDependencies(m_gamesToUpload.first())) {
                uploadGame(m_gamesToUpload.first());
            }
            else {
                if(!m_roundsToUpload.isEmpty()) {
                    uploadRound(m_roundsToUpload.first());
                }
                else {
                    m_uploadingRound = false;
                }
            }
        }
        else {
            uploadedRound->setParseUpdated(false);
            Qp::update(uploadedRound);
            m_roundsToUpload.insert(0, uploadedRound);
        }
    }
    else {
        qDebug() << "ParseController::onRoundUploadFinished: Failure" <<reply->errorString();
        m_uploadingRound = false;
    }
}

void ParseController::onPlaceUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray replyArray = reply->readAll();
        if(replyArray.isEmpty()) {
            return;
        }
        if(m_placesToUpload.isEmpty()) {
            return;
        }

        QSharedPointer<Place> uploadedPlace = m_placesToUpload.takeFirst();
        onParseObjectUploadFinished(replyArray, uploadedPlace);

        if(!m_placesToUpload.isEmpty()) {
            uploadPlace(m_placesToUpload.first());
        }
        else {
            m_uploadingPlace = false;
        }

        if(m_uploadingGame) {
            if(checkGameUpdateDependencies(m_gamesToUpload.first())) {
                uploadGame(m_gamesToUpload.first());
            }
        }
    }
    else {
        qDebug() << "Failure uploading place: " << reply->errorString();
        m_uploadingPlace = false;
    }

    reply->deleteLater();
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

    uploadParseObject(_place, m_placeNetworkManager);
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

    uploadParseObject(_player, m_playerNetworkManager);
}

void ParseController::onPlayerUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray replyArray = reply->readAll();
        if(replyArray.isEmpty()) {
            return;
        }
        if(m_playersToUpload.isEmpty()) {
            return;
        }

        QSharedPointer<Player> uploadedPlayer = m_playersToUpload.takeFirst();
        onParseObjectUploadFinished(replyArray, uploadedPlayer);

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
        qDebug() << "Failure uploading player: " <<reply->errorString();
        m_uploadingPlayer = false;
    }

    reply->deleteLater();
}

void ParseController::tryToUploadSchmeisserei(QSharedPointer<Schmeisserei> _schmeisserei)
{
    if(!m_schmeissereisToUpload.contains(_schmeisserei)) {
        m_schmeissereisToUpload.append(_schmeisserei);
    }

    if(!m_uploadingSchmeisserei) {
        m_uploadingSchmeisserei = true;
        uploadSchmeisserei(_schmeisserei);
    }
}

void ParseController::uploadSchmeisserei(QSharedPointer<Schmeisserei> _schmeisserei)
{
    connect(m_schmeissereiNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onSchmeissereiUploadFinished(QNetworkReply*)));

    uploadParseObject(_schmeisserei, m_schmeissereiNetworkManager);
}

void ParseController::onSchmeissereiUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray replyArray = reply->readAll();
        if(replyArray.isEmpty()) {
            return;
        }
        if(m_schmeissereisToUpload.isEmpty()) {
            return;
        }

        QSharedPointer<Schmeisserei> uploaded = m_schmeissereisToUpload.takeFirst();
        onParseObjectUploadFinished(replyArray, uploaded);

        if(!m_schmeissereisToUpload.isEmpty()) {
            uploadSchmeisserei(m_schmeissereisToUpload.first());
        }
        else {
            m_uploadingSchmeisserei = false;
        }

        if(m_uploadingRound && checkRoundUpdateDependencies(uploaded->round())) {
            uploadRound(uploaded->round());
        }
    }
    else {
        qDebug() << "Failure uploading schmeisserei: " << reply->errorString();
        m_uploadingSchmeisserei = false;
    }

    reply->deleteLater();
}

void ParseController::tryToUploadDrink(QSharedPointer<Drink> _drink)
{
    if(!m_drinksToUpload.contains(_drink)) {
        m_drinksToUpload.append(_drink);
    }

    if(!m_uploadingDrink) {
        m_uploadingDrink = true;
        uploadDrink(_drink);
    }
}

void ParseController::uploadDrink(QSharedPointer<Drink> _drink)
{
    connect(m_drinkNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDrinkUploadFinished(QNetworkReply*)));

    uploadParseObject(_drink, m_drinkNetworkManager);
}

void ParseController::onDrinkUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray replyArray = reply->readAll();
        if(replyArray.isEmpty()) {
            return;
        }
        if(m_drinksToUpload.isEmpty()) {
            return;
        }

        QSharedPointer<Drink> uploaded = m_drinksToUpload.takeFirst();
        onParseObjectUploadFinished(replyArray, uploaded);

        if(!m_drinksToUpload.isEmpty()) {
            uploadDrink(m_drinksToUpload.first());
        }
        else {
            m_uploadingDrink = false;
        }

        if(m_uploadingLiveDrink) {
            uploadLiveDrink(m_liveDrinksToUpload.first());
        }
    }
    else {
        qDebug() << "Failure uploading drink: " << reply->errorString();
        m_uploadingDrink = false;
    }

    reply->deleteLater();
}

void ParseController::tryToUploadLiveDrink(QSharedPointer<LiveDrink> _drink)
{
    if(!m_liveDrinksToUpload.contains(_drink)) {
        m_liveDrinksToUpload.append(_drink);
    }

    if(!m_uploadingLiveDrink) {
        m_uploadingLiveDrink = true;
        uploadLiveDrink(_drink);
    }
}

void ParseController::uploadLiveDrink(QSharedPointer<LiveDrink> _drink)
{
    if(!checkLiveDrinkUploadDependencies(_drink)) {
        return;
    }

    connect(m_liveDrinkNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onLiveDrinkUploadFinished(QNetworkReply*)));

    uploadParseObject(_drink, m_liveDrinkNetworkManager);
}

void ParseController::onLiveDrinkUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray replyArray = reply->readAll();
        if(replyArray.isEmpty()) {
            return;
        }
        if(m_liveDrinksToUpload.isEmpty()) {
            return;
        }

        QSharedPointer<LiveDrink> uploaded = m_liveDrinksToUpload.takeFirst();
        onParseObjectUploadFinished(replyArray, uploaded);

        if(!m_liveDrinksToUpload.isEmpty()) {
            uploadLiveDrink(m_liveDrinksToUpload.first());
        }
        else {
            m_uploadingLiveDrink = false;
        }

        if(m_uploadingRound && checkRoundUpdateDependencies(uploaded->round())) {
            uploadRound(uploaded->round());
        }
    }
    else {
        qDebug() << "Failure uploading drink: " << reply->errorString();
        m_uploadingLiveDrink = false;
    }

    reply->deleteLater();
}


