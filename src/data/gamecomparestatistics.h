#ifndef GAMECOMPARESTATISTICS_H
#define GAMECOMPARESTATISTICS_H

#include <QObject>
#include <QMap>

class Game;
class Round;

typedef QPair<QSharedPointer<Game>, int> GamePair;

class Slot : public QObject
{
    Q_OBJECT
public:
    enum Context {
        Hochzeit,
        Solo,
        Pflichtsolo,
        Trumpfabgabe,
        Schweinerei,
        Schmeisserei
    };

    Slot(QObject *parent = 0);
    ~Slot();

    QMap<Context, QList<GamePair> > data();

    int placement(QSharedPointer<Game> game, Slot::Context context);

    void addRound(QSharedPointer<Round> round);

    void setPreviousSlot(Slot* slot);

    int value(QSharedPointer<Round> round, Context context);

private:
    void addValue(QSharedPointer<Round> round, Context context, int value);
    GamePair pair(Context context, QSharedPointer<Round> round);

    QMap<Context, QList<GamePair> > m_data;

    Slot* m_prevSlot;
};

class GameCompareStatistics : public QObject
{
    Q_OBJECT
public:
    explicit GameCompareStatistics(QObject *parent = 0);
    static GameCompareStatistics& instance();

    int placementOfGame(QSharedPointer<Game> game, Slot::Context context, int roundNumber);
    int value(QSharedPointer<Game> game, Slot::Context context, int roundNumber);

    void filterGames(QList<QSharedPointer<Game> > games);
    QList<QSharedPointer<Game> > games();
    void resetFilter();

    void reset();

    void initialize();

private slots:
    void updateRound(QSharedPointer<QObject> object);

signals:
    void initializeToValueCompleted(int percentage);
    void initializeFinished();

private:
    QMap<int, Slot*> m_slots;
    QList<QSharedPointer<Game> > m_games;
    QList<QSharedPointer<Game> > m_allGames;

    bool m_initialized;
    bool m_useAllGames;

    static GameCompareStatistics* m_instance;
};



#endif // GAMECOMPARESTATISTICS_H
