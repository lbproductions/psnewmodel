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

    static QString fileExtension();
    static QString defaultFileName();

    static QString fileNameFromSettings();
    static QString fileNameFromArguments();
    static QString fileNameInDropbox();
    static QString fileNameLocal();

    static bool createFileIfNotExists(const QString &fileNameFromSettings);
    static void restartAndOpenLibrary(const QString &fileNameFromSettings);

private:
    Library();

    bool setupDatabase();
    bool setupPersistence();
    bool fillCaches();

    static bool lockDatabase(const QString &databaseFilePath);

    // Caches
    QList<QSharedPointer<Player> > m_players;
    QList<QSharedPointer<Game> > m_games;
    QList<QSharedPointer<Round> > m_rounds;
    QList<QSharedPointer<LiveDrink> > m_liveDrinks;

    QString m_fileName;
    bool m_isOpen;
};

#endif // LIBRARY_H
