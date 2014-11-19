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
        uploadGames();
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
    int count = 0;

    QList<QSharedPointer<Game>> games = Qp::readAll<Game>();
    foreach(QSharedPointer<Game> game, games) {
        if((game->parseID() == "" || !game->parseUpdated()) && game->type() == Game::Doppelkopf && Qp::primaryKey(game) != 271) {
            if(game->rounds().size() > 0 && count < 1) {
                //uploadGame(game);
                count++;
            }
        }
    }
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
            //qDebug() << replyArray;

            QJsonDocument document = QJsonDocument::fromJson(replyArray);
            QJsonArray jsonObjects = document.object().value("results").toArray();
            for(int i = 0; i<jsonObjects.size(); i++) {
                QJsonObject jsonObject = jsonObjects.at(i).toObject();
                QJsonValue value = jsonObject.value("objectId");
                QString id = value.toString();
                //qDebug() << "Keys: " << jsonObject.keys();

                QSharedPointer<ParseObject> parseObject = m_cache.value(className).value(id);
                if(!parseObject) {
                    parseObject = Qp::create<T>();
                    parseObject->setParseID(id);
                    parseObject->setParseUpdated(true);
                    parseObject->setParseLastUpdate(QDateTime::currentDateTime());
                    parseObject->parseUpdateFromJSON(jsonObject, true);
                }
                else {
                    QDateTime updateTime = QDateTime::fromString(jsonObject.value("updatedAt").toString(), Qt::ISODate);
                    QDateTime parseLastUpdate = parseObject->parseLastUpdate();
                    if(updateTime > parseLastUpdate) {
                        //qDebug() << replyArray;
                        parseObject->parseUpdateFromJSON(jsonObject);
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
    url.setQuery("where={\"updatedAt\":{\"$gte\": {\"__type\":\"Date\", \"iso\":\""+ lastUpdate.toString(Qt::ISODate)
                 +"\"}, \"$lte\": {\"__type\":\"Date\", \"iso\":\""+ m_updateStartTime.toString(Qt::ISODate) +"\"}}}");
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

/*
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
        if(player->parseID() == "" || !player->parseUpdated()) {
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


*/
