#ifndef LIBRARY_H
#define LIBRARY_H

#include <data/game.h>
#include <data/player.h>
#include <data/round.h>

class Library
{
public:
    static Library *instance();
    bool open();
    void close();

    bool isOpen() const;

    QString fileName() const;
    void setFileName(const QString &fileNameFromSettings);

    QString packagePath() const;
    void setPackagePath(const QString &packagePath);

    static QString fileExtension();
    static QString defaultFileName();

    static QString fileNameFromSettings();
    static QString fileNameFromArguments();
    static QString fileNameLocal();

    static bool createFileIfNotExists(const QString &fileNameFromSettings);
    static void restartAndOpenLibrary(const QString &fileNameFromSettings);

private:
    Library();

    bool setupDatabase();
    bool setupPersistence();
    bool fillCaches();

    bool fixDatabase(QSqlDatabase db);
    bool fixGamePositions(QSqlDatabase db);
    bool fixGamePositionsFromOldDb(QSqlDatabase db, const QString &oldDb, int begin, int end);
    bool fixGamePositionsFromOldDb(QSqlDatabase db);
    bool fixGamePositionsByPosition(int game, int expectedPlayerCount, QSqlDatabase db);
    bool fixGamePositionsById(int game, int expectedPlayerCount, QSqlDatabase db);
    bool fixUniqueKeys(QSqlDatabase db, const QString &fk1, const QString &fk2);

    static bool lockDatabase(const QString &databaseFilePath);

    // Caches
    QList<QSharedPointer<Player> > m_players;
    QList<QSharedPointer<Game> > m_games;
    QList<QSharedPointer<Round> > m_rounds;
    QList<QSharedPointer<LiveDrink> > m_liveDrinks;

    QString m_fileName;
    QString m_packagePath;
    bool m_isOpen;
};

#endif // LIBRARY_H
