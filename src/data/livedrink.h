#ifndef LIVEDRINK_H
#define LIVEDRINK_H

#include <QObject>

#include <QPersistence.h>
#include <QDateTime>



class Player;
class Round;
class Drink;

class LiveDrink : public QObject
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
    void setPlayer(QSharedPointer<Player> player);

    QSharedPointer<Round> round() const;
    void setRound(QSharedPointer<Round> round);

    QSharedPointer<Drink> drink() const;
    void setDrink(QSharedPointer<Drink> drink);

    void clearRelations();

private:
    void setTime(const QDateTime &time);

    QDateTime m_time;

    QpHasOne<Player> m_player;
    QpBelongsToOne<Round> m_round;
    QpHasOne<Drink> m_drink;
};



#endif // LIVEDRINK_H
