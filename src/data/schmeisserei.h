#ifndef SCHMEISSEREI_H
#define SCHMEISSEREI_H

#include <QObject>

#include <QPersistence.h>



class Round;
class Player;

class Schmeisserei : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(Type type READ type WRITE setType)
    Q_PROPERTY(QSharedPointer<Round> round READ round WRITE setRound)
    Q_PROPERTY(QSharedPointer<Player> player READ player WRITE setPlayer)
    Q_PROPERTY(int kingsCount READ kingsCount WRITE setKingsCount)
    Q_PROPERTY(int trumpfCount READ trumpfCount WRITE setTrumpfCount)
    Q_PROPERTY(int points READ points WRITE setPoints)


    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:player",
                "reverserelation=schmeissereien")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:round",
                "reverserelation=schmeissereien")

public:
    enum Type {
        UnkownType,
        MoreThan4Kings,
        NoTrumpfabgabeTaker,
        LessThan3Trumpf,
        MoreThan90Points,
        NothingOverFox
    };

    explicit Schmeisserei(QObject *parent = 0);

    Type type() const;
    void setType(const Type &type);
    QString typeString() const;

    QSharedPointer<Round> round() const;
    void setRound(const QSharedPointer<Round> &round);

    QSharedPointer<Player> player() const;
    void setPlayer(const QSharedPointer<Player> &player);

    static QStringList typeStrings();
    static QString typeStringFromType(Type type);
    static Type typeFromString(const QString &typeString);

    int kingsCount() const;
    void setKingsCount(int arg);

    int trumpfCount() const;
    void setTrumpfCount(int arg);

    int points() const;
    void setPoints(int arg);

private:
    Type m_type;
    QpWeakRelation<Round> m_round;
    QpWeakRelation<Player> m_player;
    int m_kingsCount;
    int m_trumpfabgabeCount;
    int m_points;
};



#endif // SCHMEISSEREI_H
