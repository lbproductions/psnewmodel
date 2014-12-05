#ifndef PARSECONTROLLER_H
#define PARSECONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include <QDateTime>

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
class QJsonValueRef;

class ParseController : public QObject
{
    Q_OBJECT
public:
    explicit ParseController(QObject *parent = 0);

    static ParseController *instance();

    void syncData();

    template<class T>
    QSharedPointer<T> objectFromCache(QString _objectID);
    void addToCache(QSharedPointer<ParseObject> object);

public slots:
    void update();
    void uploadGames();
    void uploadGame(QSharedPointer<Game> game);

private slots:
    void uploadNextGame();

signals:
    void updateFinished();
    void gamesUploadFinished();

private:
    void createCaches();
    void _update();

    template<class T>
    void updateClass();

    template<class T>
    void registerCacheClass();

    QList<QJsonValue> sortJsonArrayToCreationTime(const QJsonArray &array);

    QHash<QString, QHash<QString, QSharedPointer<ParseObject>>> m_cache;
    QList<QString> m_updatedClass;

    QList<QSharedPointer<Game>> m_uploadingGames;

    bool m_isUpdating;
    bool m_isUploading;

    QDateTime m_updateStartTime;
};

bool sortParseObjectsByCreationTime(QSharedPointer<ParseObject> obj1, QSharedPointer<ParseObject> obj2);

template<class T>
QSharedPointer<T> ParseController::objectFromCache(QString _objectID)
{
    QSharedPointer<ParseObject> parseObject = m_cache.value(T::staticMetaObject.className()).value(_objectID);
    if(parseObject) {
        return qSharedPointerCast<T>(parseObject);
    }

    return QSharedPointer<T>();
}

#endif // PARSECONTROLLER_H
