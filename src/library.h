#ifndef LIBRARY_H
#define LIBRARY_H

#include <data/game.h>
#include <data/player.h>
#include <data/round.h>

class Library
{
public:
    Library();

    static bool open();
    static void close();

    static QString currentFileName();
    static void saveFileNameInSettings(const QString &fileName);

    static QString fileExtension();
    static QString defaultFileName();

    static void openLibrary(const QString &fileName);

private:
    static bool setupDatabase();
    static bool setupPersistence();
    static bool fillCaches();

    static bool lockDatabase(const QString &databaseFilePath);

    static QString getDatabaseFile();

    static QString fileNameFromArguments();
    static QString fileNameInDropbox();
    static QString fileNameLocal();

    static bool createFileIfNotExists(const QString &fileName);

    // Caches
    static QList<QSharedPointer<Player> > m_players;
    static QList<QSharedPointer<Game> > m_games;
    static QList<QSharedPointer<Round> > m_rounds;
};

#endif // LIBRARY_H
