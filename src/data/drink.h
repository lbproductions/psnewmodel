#ifndef DRINK_H
#define DRINK_H

#include "parseobject.h"
#include <QPersistence.h>
#include <QPixmap>



class LiveDrink;
class Player;

class Drink : public ParseObject
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(Type type READ type WRITE setType)
    Q_PROPERTY(double volume READ volume WRITE setVolume)
    Q_PROPERTY(QPixmap picture READ picture WRITE setPicture)
    Q_PROPERTY(QString iconPath READ iconPath WRITE setIconPath)
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
    QString typeString() const;

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

    static QStringList typeStrings();
    static QString typeStringFromType(Type type);
    static Type typeFromString(const QString &typeString);

private slots:
    void setLiveDrinks(const QList<QSharedPointer<LiveDrink> > &drinks);
    void addLiveDrink(QSharedPointer<LiveDrink> drink);
    void removeLiveDrink(QSharedPointer<LiveDrink> drink);

private:
    void setIconPath(QString path);
    QString iconPath() const;

    QString m_name;
    Type m_type;
    double m_volume;
    QPixmap m_picture;
    double m_alcoholByVolume;
    QString m_iconPath;

    QpBelongsToMany<LiveDrink> m_liveDrinks;

    // ParseObject interface
public:
    QByteArray parseJSONData();

    bool parseCheckAfterUploadConditions();

    void parseUpdateFromJSON(QJsonObject object, bool created = false);
};




#endif // DRINK_H
