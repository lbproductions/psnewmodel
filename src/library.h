#ifndef LIBRARY_H
#define LIBRARY_H

#include <data/game.h>
#include <data/player.h>
#include <data/round.h>

class Library
{
public:
    Library();

    bool open();
    void close();

private:
    bool setupDatabase();
    bool setupPersistence();
    bool fillCaches();

    QString getDatabaseFile() const;
    void saveFileNameInSettings(const QString &fileName) const;

    QString fileNameFromArguments() const;
    QString fileNameFromSettings() const;
    QString fileNameInDropbox() const;
    QString fileNameLocal() const;

    bool createFileIfNotExists(const QString &fileName) const;

    // Caches
    QList<QSharedPointer<Player> > m_players;
    QList<QSharedPointer<Game> > m_games;
    QList<QSharedPointer<Round> > m_rounds;
};

#endif // LIBRARY_H
