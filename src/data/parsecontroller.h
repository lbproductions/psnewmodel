#ifndef PARSECONTROLLER_H
#define PARSECONTROLLER_H

#include <QObject>
#include <QSharedPointer>

class Game;
class Round;
class Player;
class Place;
class ParseObject;

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

public slots:
    void onGameUploadFinished(QNetworkReply* reply);
    void onRoundUploadFinished(QNetworkReply* reply);
    void onPlayerUploadFinished(QNetworkReply* reply);
    void onPlaceUploadFinished(QNetworkReply* reply);

private:
    void uploadGame(QSharedPointer<Game> _game);
    void uploadRound(QSharedPointer<Round> _round);
    void uploadPlayer(QSharedPointer<Player> _player);
    void uploadPlace(QSharedPointer<Place> _place);

    bool checkGameDependendUploads(QSharedPointer<Game> _game);

    QNetworkAccessManager* m_gameNetworkManager;
    QNetworkAccessManager* m_roundNetworkManager;
    QNetworkAccessManager* m_playerNetworkManager;
    QNetworkAccessManager* m_placeNetworkManager;

    bool m_uploadingGame;
    bool m_uploadingRound;
    bool m_uploadingPlayer;
    bool m_uploadingPlace;

    QList<QSharedPointer<Game>> m_gamesToUpload;
    QList<QSharedPointer<Round>> m_roundsToUpload;
    QList<QSharedPointer<Player>> m_playersToUpload;
    QList<QSharedPointer<Place>> m_placesToUpload;
};

#endif // PARSECONTROLLER_H
