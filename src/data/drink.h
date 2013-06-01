#ifndef DRINK_H
#define DRINK_H

#include <QObject>

#include <QPersistenceRelations.h>
#include <QPixmap>



class LiveDrink;
class Player;

class Drink : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int type READ _type WRITE _setType)
    Q_PROPERTY(double volume READ volume WRITE setVolume)
    Q_PROPERTY(QPixmap picture READ picture WRITE setPicture)
    Q_PROPERTY(double alcoholByVolume READ alcoholByVolume WRITE setAlcoholByVolume)
    Q_PROPERTY(QList<QSharedPointer<LiveDrink> > liveDrinks READ liveDrinks WRITE setLiveDrinks)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:liveDrinks",
                "reverserelation=drink")

public:
    enum Type {
        UnkownType,
        Beer,
        MixedBeer,
        LongDrink,
        Liquor,
        Wine
    };

    explicit Drink(QObject *parent = 0);
    ~Drink();
    
    QString name() const;
    void setName(const QString &name);

    Type type() const;
    void setType(const Type &type);

    double volume() const;
    void setVolume(double volume);

    QPixmap picture() const;
    void setPicture(const QPixmap &picture);

    double alcoholByVolume() const;
    void setAlcoholByVolume(double alcoholByVolume);

    QList<QSharedPointer<LiveDrink> > liveDrinks() const;

    int count() const;
    int count(QSharedPointer<Player> player) const;

    QList<QSharedPointer<Player> > playersSortedByCount() const;


private:
    void setLiveDrinks(const QList<QSharedPointer<LiveDrink> > &drinks);

    int _type() const;
    void _setType(int type);

    QString m_name;
    Type m_type;
    double m_volume;
    QPixmap m_picture;
    double m_alcoholByVolume;

    QpWeakRelation<LiveDrink> m_liveDrinks;
};



#endif // DRINK_H
