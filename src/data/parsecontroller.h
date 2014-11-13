#ifndef PARSECONTROLLER_H
#define PARSECONTROLLER_H

#include <QObject>
#include <QSharedPointer>

class Game;
class Round;
class Player;
class Place;
class ParseObject;
class Schmeisserei;
class Drink;
class LiveDrink;

class QNetworkAccessManager;
class QNetworkReply;

class ParseController : public QObject
{
    Q_OBJECT
public:
    explicit ParseController(QObject *parent = 0);

    static ParseController *instance();

    void uploadGames();

    void tryToUploadGame(QSharedPointer<Game> _game);
    void tryToUploadRound(QSharedPointer<Round> _round);
    void tryToUploadPlace(QSharedPointer<Place> _place);
    void tryToUploadPlayer(QSharedPointer<Player> _player);
    void tryToUploadSchmeisserei(QSharedPointer<Schmeisserei> _schmeisserei);
    void tryToUploadDrink(QSharedPointer<Drink> _drink);
    void tryToUploadLiveDrink(QSharedPointer<LiveDrink> _drink);

public slots:
    void onGameUploadFinished(QNetworkReply* reply);
    void onRoundUploadFinished(QNetworkReply* reply);
    void onPlayerUploadFinished(QNetworkReply* reply);
    void onPlaceUploadFinished(QNetworkReply* reply);
    void onSchmeissereiUploadFinished(QNetworkReply* reply);
    void onDrinkUploadFinished(QNetworkReply* reply);
    void onLiveDrinkUploadFinished(QNetworkReply* reply);

    void onParseObjectUploadFinished(QByteArray replyArray, QSharedPointer<ParseObject> _object);

private:
    void uploadGame(QSharedPointer<Game> _game);
    void uploadRound(QSharedPointer<Round> _round);
    void uploadPlayer(QSharedPointer<Player> _player);
    void uploadPlace(QSharedPointer<Place> _place);
    void uploadSchmeisserei(QSharedPointer<Schmeisserei> _schmeisserei);
    void uploadDrink(QSharedPointer<Drink> _drink);
    void uploadLiveDrink(QSharedPointer<LiveDrink> _drink);
    void uploadParseObject(QSharedPointer<ParseObject> _object, QNetworkAccessManager *_networkManager);

    bool checkGameDependendUploads(QSharedPointer<Game> _game);
    bool checkGameUpdateDependencies(QSharedPointer<Game> _game);
    bool checkRoundUploadDependencies(QSharedPointer<Round> _round);
    bool checkRoundUpdateDependencies(QSharedPointer<Round> _round);
    bool checkLiveDrinkUploadDependencies(QSharedPointer<LiveDrink> _drink);

    QNetworkAccessManager* m_gameNetworkManager;
    QNetworkAccessManager* m_roundNetworkManager;
    QNetworkAccessManager* m_playerNetworkManager;
    QNetworkAccessManager* m_placeNetworkManager;
    QNetworkAccessManager* m_schmeissereiNetworkManager;
    QNetworkAccessManager* m_drinkNetworkManager;
    QNetworkAccessManager* m_liveDrinkNetworkManager;

    bool m_uploadingGame;
    bool m_uploadingRound;
    bool m_uploadingPlayer;
    bool m_uploadingPlace;
    bool m_uploadingSchmeisserei;
    bool m_uploadingDrink;
    bool m_uploadingLiveDrink;

    QList<QSharedPointer<Game>> m_gamesToUpload;
    QList<QSharedPointer<Round>> m_roundsToUpload;
    QList<QSharedPointer<Player>> m_playersToUpload;
    QList<QSharedPointer<Place>> m_placesToUpload;
    QList<QSharedPointer<Schmeisserei>> m_schmeissereisToUpload;
    QList<QSharedPointer<Drink>> m_drinksToUpload;
    QList<QSharedPointer<LiveDrink>> m_liveDrinksToUpload;
};

#endif // PARSECONTROLLER_H
