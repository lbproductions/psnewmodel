#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

#include <QColor>
#include <QPixmap>
#include <QPersistenceRelations.h>



class Schmeisserei;
class Round;
class Place;
class LiveDrink;
class Game;
class Point;

class Player : public QObject
{
    Q_OBJECT
    Q_ENUMS(Gender)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int gender READ _gender WRITE _setGender)
    Q_PROPERTY(int weight READ weight WRITE setWeight)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QPixmap avatar READ avatar WRITE setAvatar)
    Q_PROPERTY(QList<QSharedPointer<Place> > places READ places WRITE setPlaces)
    Q_PROPERTY(QList<QSharedPointer<LiveDrink> > liveDrinks READ liveDrinks WRITE setLiveDrinks)
    Q_PROPERTY(QList<QSharedPointer<Game> > games READ games WRITE setGames)
    Q_PROPERTY(QList<QSharedPointer<Schmeisserei> > schmeissereien READ schmeissereien WRITE setSchmeissereien)

    Q_PROPERTY(QList<QSharedPointer<Round> > hochzeitRounds READ hochzeitRounds WRITE setHochzeitRounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > trumpfabgabeRounds READ trumpfabgabeRounds WRITE setTrumpfabgabeRounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > soloRounds READ soloRounds WRITE setSoloRounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > schweinereiRounds READ schweinereiRounds WRITE setSchweinereiRounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > re1Rounds READ re1Rounds WRITE setRe1Rounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > re2Rounds READ re2Rounds WRITE setRe2Rounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > contra1Rounds READ contra1Rounds WRITE setContra1Rounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > contra2Rounds READ contra2Rounds WRITE setContra2Rounds)
    Q_PROPERTY(QList<QSharedPointer<Round> > contra3Rounds READ contra3Rounds WRITE setContra3Rounds)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:places",
                "reverserelation=players")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:liveDrinks",
                "reverserelation=player")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:games",
                "reverserelation=players")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:schmeissereien",
                "reverserelation=player")

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

    int weight() const;
    void setWeight(int weight);

    int height() const;
    void setHeight(int height);

    QColor color() const;
    void setColor(const QColor &color);

    QPixmap avatar() const;
    void setAvatar(const QPixmap &avatar);

    QList<QSharedPointer<Place> > places() const;
    void addPlace(QSharedPointer<Place> place);

    QList<QSharedPointer<LiveDrink> > liveDrinks() const;
    QList<QSharedPointer<Game> > games() const;

    int gamePoints() const;
    int points() const;
    double average() const;

    int wins() const;
    int losses() const;
    double averagePlacement() const;

    QSharedPointer<Game> lastGame() const;
    QSharedPointer<Game> lastWin() const;

//    DECLARE_MAPPINGATTRIBUTE_IN_CALC(LiveGame*,double,Player,PlayerCalculator,alcPegel)

    QList<QSharedPointer<Schmeisserei> > schmeissereien() const;
    QList<QSharedPointer<Round> > rounds() const;

    QList<QSharedPointer<Round> > hochzeitRounds() const;
    QList<QSharedPointer<Round> > trumpfabgabeRounds() const;
    QList<QSharedPointer<Round> > soloRounds() const;
    QList<QSharedPointer<Round> > schweinereiRounds() const;
    QList<QSharedPointer<Round> > re1Rounds() const;
    QList<QSharedPointer<Round> > re2Rounds() const;

    QList<QSharedPointer<Round> > reRounds() const;
    double rePercentage() const;
    QList<QSharedPointer<Round> > reWins() const;
    double reWinsPercentage() const;

    QList<QSharedPointer<Round> > contraRounds() const;
    double contraPercentage() const;
    QList<QSharedPointer<Round> > contraWins() const;
    double contraWinsPercentage() const;

    double averagePointsPerRound() const;
    QList<QSharedPointer<Round> > winRounds() const;
    double roundWinsPercentage() const;

    QList<QSharedPointer<Round> > contra1Rounds() const;
    void setContra1Rounds(QList<QSharedPointer<Round> > arg);

    QList<QSharedPointer<Round> > contra2Rounds() const;
    void setContra2Rounds(QList<QSharedPointer<Round> > arg);

    QList<QSharedPointer<Round> > contra3Rounds() const;
    void setContra3Rounds(QList<QSharedPointer<Round> > arg);

private:
    void setPlaces(const QList<QSharedPointer<Place> > &places);
    void setLiveDrinks(const QList<QSharedPointer<LiveDrink> > &drinks);
    void setGames(const QList<QSharedPointer<Game> > &games);
    void setSchmeissereien(const QList<QSharedPointer<Schmeisserei> > &schmeissereien);
    void setHochzeitRounds(const QList<QSharedPointer<Round> > &hochzeitRounds);
    void setTrumpfabgabeRounds(const QList<QSharedPointer<Round> > &trumpfabgabeRounds);
    void setSoloRounds(const QList<QSharedPointer<Round> > &soloRounds);
    void setSchweinereiRounds(const QList<QSharedPointer<Round> > &schweinereiRounds);
    void setRe1Rounds(const QList<QSharedPointer<Round> > &re1Rounds);
    void setRe2Rounds(const QList<QSharedPointer<Round> > &re2Rounds);

    int _gender() const;
    void _setGender(int gender);

    QString m_name;
    Gender m_gender;
    int m_weight;
    int m_height;
    QColor m_color;
    QPixmap m_avatar;

    QpWeakRelation<Place> m_places;
    QpWeakRelation<LiveDrink> m_liveDrinks;
    QpWeakRelation<Game> m_games;
    QpStrongRelation<Schmeisserei> m_schmeissereien;

    QpWeakRelation<Round> m_hochzeitRounds;
    QpWeakRelation<Round> m_trumpfabgabeRounds;
    QpWeakRelation<Round> m_soloRounds;
    QpWeakRelation<Round> m_schweinereiRounds;
    QpWeakRelation<Round> m_re1Rounds;
    QpWeakRelation<Round> m_re2Rounds;
    QpWeakRelation<Round> m_contra1Rounds;
    QpWeakRelation<Round> m_contra2Rounds;
    QpWeakRelation<Round> m_contra3Rounds;
};



#endif // PLAYER_H
