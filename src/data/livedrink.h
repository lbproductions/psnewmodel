#ifndef LIVEDRINK_H
#define LIVEDRINK_H

#include "parseobject.h"
#include <QPersistence.h>
#include <QDateTime>



class Player;
class Round;
class Drink;

class LiveDrink : public ParseObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime time READ time WRITE setTime)
    Q_PROPERTY(QSharedPointer<Player> player READ player WRITE setPlayer)
    Q_PROPERTY(QSharedPointer<Round> round READ round WRITE setRound)
    Q_PROPERTY(QSharedPointer<Drink> drink READ drink WRITE setDrink)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:player",
                "reverserelation=liveDrinks")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:round",
                "reverserelation=liveDrinks")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:drink",
                "reverserelation=liveDrinks")

public:
    explicit LiveDrink(QObject *parent = 0);
    ~LiveDrink();

    QDateTime time() const;

    QSharedPointer<Player> player() const;
    QSharedPointer<Round> round() const;
    QSharedPointer<Drink> drink() const;

    void clearRelations();

public slots:
    void setPlayer(QSharedPointer<Player> player);
    void setRound(QSharedPointer<Round> round);
    void setDrink(QSharedPointer<Drink> drink);

private:
    void setTime(const QDateTime &time);

    QDateTime m_time;

    QpHasOne<Player> m_player;
    QpBelongsToOne<Round> m_round;
    QpHasOne<Drink> m_drink;

    // ParseObject interface
public:
    QByteArray JSONData();
};



#endif // LIVEDRINK_H
