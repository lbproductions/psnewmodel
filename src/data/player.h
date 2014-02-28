#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

#include <QColor>
#include <QPixmap>
#include <QPersistence.h>



class Schmeisserei;
class Round;
class Place;
class LiveDrink;
class Game;
class Point;
class PlayerStatistics;
class League;
class Drink;
class OLD_OfflineGameInformation;

class Player : public QObject
{
    Q_OBJECT
    Q_ENUMS(Gender)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(Gender gender READ gender WRITE setGender)
    Q_PROPERTY(int weight READ weight WRITE setWeight)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QPixmap avatar READ avatar WRITE setAvatar)
    Q_PROPERTY(QList<QSharedPointer<Place> > places READ places WRITE setPlaces)
    Q_PROPERTY(QList<QSharedPointer<LiveDrink> > liveDrinks READ liveDrinks WRITE setLiveDrinks)
    Q_PROPERTY(QList<QSharedPointer<Game> > games READ games WRITE setGames)
    Q_PROPERTY(QList<QSharedPointer<Schmeisserei> > schmeissereis READ schmeissereis WRITE setSchmeissereis)

    Q_PROPERTY(QList<QSharedPointer<Round> > hochzeitRounds READ hochzeitRounds WRITE setHochzeitRounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > trumpfabgabeRounds READ trumpfabgabeRounds WRITE setTrumpfabgabeRounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > soloRounds READ soloRounds WRITE setSoloRounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > schweinereiRounds READ schweinereiRounds WRITE setSchweinereiRounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > re1Rounds READ re1Rounds WRITE setRe1Rounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > re2Rounds READ re2Rounds WRITE setRe2Rounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > contra1Rounds READ contra1Rounds WRITE setContra1Rounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > contra2Rounds READ contra2Rounds WRITE setContra2Rounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > contra3Rounds READ contra3Rounds WRITE setContra3Rounds)
    Q_PROPERTY(QList<QSharedPointer<League> > leagues READ leagues WRITE setLeagues)

    Q_PROPERTY(QList<QSharedPointer<OLD_OfflineGameInformation> > offlineGameInformation READ offlineGameInformation WRITE setOfflineGameInformation)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:places",
                "reverserelation=players")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:liveDrinks",
                "reverserelation=player")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:games",
                "reverserelation=players")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:schmeissereis",
                "reverserelation=player")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:leagues",
                "reverserelation=players")

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:hochzeitRounds",
                "reverserelation=hochzeitPlayer")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:trumpfabgabeRounds",
                "reverserelation=trumpfabgabePlayer")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:soloRounds",
                "reverserelation=soloPlayer")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:schweinereiRounds",
                "reverserelation=schweinereiPlayer")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:re1Rounds",
                "reverserelation=re1Player")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:re2Rounds",
                "reverserelation=re2Player")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:contra1Rounds",
                "reverserelation=contra1Player")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:contra2Rounds",
                "reverserelation=contra2Player")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:contra3Rounds",
                "reverserelation=contra3Player")

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:offlineGameInformation",
                "reverserelation=player")

public:
    enum Gender {
        UnkownGender,
        Male,
        Female
    };

    explicit Player(QObject *parent = 0);
    ~Player();

    QString name() const;
    void setName(const QString &name);

    Gender gender() const;
    void setGender(const Gender &gender);
    QString genderString() const;

    int weight() const;
    void setWeight(int weight);

    int height() const;
    void setHeight(int height);

    QColor color() const;
    void setColor(const QColor &color);
    QPixmap colorPixmap(int w = 16, int h = 16) const;

    QPixmap avatar() const;
    void setAvatar(const QPixmap &avatar);

    QList<QSharedPointer<Place> > places() const;
    QList<QSharedPointer<LiveDrink> > liveDrinks() const;
    QList<QSharedPointer<Drink> > drinks() const;
    QList<QSharedPointer<League> > leagues() const;
    QList<QSharedPointer<Game> > games() const;
    QSharedPointer<Game> lastGame() const;
    QSharedPointer<Game> lastWin() const;

    QList<QSharedPointer<Round> > rounds() const;
    QList<QSharedPointer<Round> > winRounds() const;
    QList<QSharedPointer<Round> > reRounds() const;
    QList<QSharedPointer<Round> > reWins() const;
    QList<QSharedPointer<Round> > contraRounds() const;
    QList<QSharedPointer<Round> > contraWins() const;
    QList<QSharedPointer<Round> > hochzeitRounds() const;
    QList<QSharedPointer<Round> > trumpfabgabeRounds() const;
    QList<QSharedPointer<Round> > soloRounds() const;
    QList<QSharedPointer<Round> > schweinereiRounds() const;
    QList<QSharedPointer<Schmeisserei> > schmeissereis() const;
    QList<QSharedPointer<OLD_OfflineGameInformation> > offlineGameInformation() const;

    double roundWinsPercentage() const;
    double rePercentage() const;
    double reWinsPercentage() const;
    double contraPercentage() const;
    double contraWinsPercentage() const;

    double averagePointsPerRound() const;
    double averagePlacement() const;

    int gamePoints() const;

    int points() const;
    double average() const;

    int wins() const;
    int losses() const;

    QSharedPointer<PlayerStatistics> allGamesStatistics() const;

private slots:
    void setRe1Rounds(const QList<QSharedPointer<Round> > &re1Rounds);
    void setRe2Rounds(const QList<QSharedPointer<Round> > &re2Rounds);
    void setContra1Rounds(QList<QSharedPointer<Round> > arg);
    void setContra2Rounds(QList<QSharedPointer<Round> > arg);
    void setContra3Rounds(QList<QSharedPointer<Round> > arg);

    void addRe1Round(QSharedPointer<Round> round);
    void addRe2Round(QSharedPointer<Round> round);
    void addContra1Round(QSharedPointer<Round> round);
    void addContra2Round(QSharedPointer<Round> round);
    void addContra3Round(QSharedPointer<Round> round);

    void removeRe1Round(QSharedPointer<Round> round);
    void removeRe2Round(QSharedPointer<Round> round);
    void removeContra1Round(QSharedPointer<Round> round);
    void removeContra2Round(QSharedPointer<Round> round);
    void removeContra3Round(QSharedPointer<Round> round);

    void setLiveDrinks(const QList<QSharedPointer<LiveDrink> > &drinks);
    void addLiveDrink(QSharedPointer<LiveDrink> drink);
    void removeLiveDrink(QSharedPointer<LiveDrink> drink);

    void setPlaces(const QList<QSharedPointer<Place> > &places);
    void addPlace(QSharedPointer<Place> place);
    void removePlace(QSharedPointer<Place> place);

    void setGames(const QList<QSharedPointer<Game> > &games);
    void addGame(QSharedPointer<Game> game);
    void removeGame(QSharedPointer<Game> game);

    void setSchmeissereis(const QList<QSharedPointer<Schmeisserei> > &schmeissereis);
    void addSchmeisserei(QSharedPointer<Schmeisserei> schmeisserei);
    void removeSchmeisserei(QSharedPointer<Schmeisserei> schmeisserei);

    void setHochzeitRounds(const QList<QSharedPointer<Round> > &hochzeitRounds);
    void addHochzeitRound(QSharedPointer<Round> hochzeitRound);
    void removeHochzeitRound(QSharedPointer<Round> hochzeitRound);

    void setTrumpfabgabeRounds(const QList<QSharedPointer<Round> > &trumpfabgabeRounds);
    void addTrumpfabgabeRound(QSharedPointer<Round> trumpfabgabeRound);
    void removeTrumpfabgabeRound(QSharedPointer<Round> trumpfabgabeRound);

    void setSoloRounds(const QList<QSharedPointer<Round> > &soloRounds);
    void addSoloRound(QSharedPointer<Round> soloRound);
    void removeSoloRound(QSharedPointer<Round> soloRound);

    void setSchweinereiRounds(const QList<QSharedPointer<Round> > &schweinereiRounds);
    void addSchweinereiRound(QSharedPointer<Round> schweinereiRounds);
    void removeSchweinereiRound(QSharedPointer<Round> schweinereiRounds);

    void setLeagues(const QList<QSharedPointer<League> > &arg);
    void addLeague(QSharedPointer<League> arg);
    void removeLeague(QSharedPointer<League> arg);

    void setOfflineGameInformation(const QList<QSharedPointer<OLD_OfflineGameInformation> > &games);
    void addOfflineGameInformation(QSharedPointer<OLD_OfflineGameInformation> game);
    void removeOfflineGameInformation(QSharedPointer<OLD_OfflineGameInformation> game);

private:
    friend class PlayersListModel;
    QList<QSharedPointer<Round> > re1Rounds() const;
    QList<QSharedPointer<Round> > re2Rounds() const;
    QList<QSharedPointer<Round> > contra1Rounds() const;
    QList<QSharedPointer<Round> > contra2Rounds() const;
    QList<QSharedPointer<Round> > contra3Rounds() const;

    QString m_name;
    Gender m_gender;
    int m_weight;
    int m_height;
    QColor m_color;
    QPixmap m_avatar;

    QSharedPointer<PlayerStatistics> m_allGamesStatistics;

    mutable QList<QSharedPointer<Round>> m_roundsCached;
    mutable QList<QSharedPointer<Round>> m_contraRoundsCached;
    mutable QList<QSharedPointer<Round>> m_reRoundsCached;

    QpHasMany<Place> m_places;
    QpBelongsToMany<LiveDrink> m_liveDrinks;
    QpBelongsToMany<League> m_leagues;
    QpBelongsToMany<Game> m_games;
    QpBelongsToMany<Schmeisserei> m_schmeissereis;

    QpBelongsToMany<Round> m_hochzeitRounds;
    QpBelongsToMany<Round> m_trumpfabgabeRounds;
    QpBelongsToMany<Round> m_soloRounds;
    QpBelongsToMany<Round> m_schweinereiRounds;
    QpBelongsToMany<Round> m_re1Rounds;
    QpBelongsToMany<Round> m_re2Rounds;
    QpBelongsToMany<Round> m_contra1Rounds;
    QpBelongsToMany<Round> m_contra2Rounds;
    QpBelongsToMany<Round> m_contra3Rounds;

    QpBelongsToMany<OLD_OfflineGameInformation> m_offlineGameInformation;
};



#endif // PLAYER_H
