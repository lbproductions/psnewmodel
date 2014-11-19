#include "drink.h"

#include "livedrink.h"



Drink::Drink(QObject *parent) :
    ParseObject(parent),
    m_type(UnkownType),
    m_volume(0.0),
    m_alcoholByVolume(0.0),
    m_liveDrinks(QpRelation(&Drink::liveDrinks))
{
    setPicture(QPixmap(iconPath()));
}

Drink::~Drink()
{
}

double Drink::alcoholByVolume() const
{
    return m_alcoholByVolume;
}

void Drink::setAlcoholByVolume(double alcoholByVolume)
{
    m_alcoholByVolume = alcoholByVolume;
}

QList<QSharedPointer<LiveDrink> > Drink::liveDrinks() const
{
    return m_liveDrinks;
}

void Drink::setLiveDrinks(const QList<QSharedPointer<LiveDrink> > &drinks)
{
    m_liveDrinks = drinks;
}

void Drink::addLiveDrink(QSharedPointer<LiveDrink> drink)
{
    m_liveDrinks.add(drink);
}

void Drink::removeLiveDrink(QSharedPointer<LiveDrink> drink)
{
    m_liveDrinks.remove(drink);
}

void Drink::setIconPath(QString path)
{
    m_iconPath = path;
}

QString Drink::iconPath() const
{
    return m_iconPath;
}

int Drink::count() const
{
    return liveDrinks().size();
}

int Drink::count(QSharedPointer<Player> player) const
{
    int count = 0;
    foreach(QSharedPointer<LiveDrink> drink, liveDrinks()) {
        if(drink->player() == player) {
            ++count;
        }
    }
    return count;
}

QList<QSharedPointer<Player> > Drink::playersSortedByCount() const
{
    QMap<QSharedPointer<Player>, int> counts;

    foreach(QSharedPointer<LiveDrink> drink, liveDrinks()) {
        ++counts[drink->player()];
    }

    QMultiMap<int, QSharedPointer<Player> > helperMap;
    for(auto it = counts.constBegin(); it != counts.constEnd(); ++it) {
        helperMap.insert(it.value(), it.key());
    }

    return helperMap.values();
}


QPixmap Drink::picture() const
{
    return m_picture;
}

void Drink::setPicture(const QPixmap &picture)
{
    m_picture = picture;
}

double Drink::volume() const
{
    return m_volume;
}

void Drink::setVolume(double volume)
{
    m_volume = volume;
}

Drink::Type Drink::type() const
{
    return m_type;
}

void Drink::setType(const Type &type)
{
    m_type = type;
}

QString Drink::typeString() const
{
    return typeStringFromType(type());
}

QString Drink::name() const
{
    return m_name;
}

void Drink::setName(const QString &name)
{
    m_name = name;
}


QStringList Drink::typeStrings()
{
    return QStringList() << tr("Beer") <<
                            tr("Mixed beer") <<
                            tr("Long drink") <<
                            tr("Liquor") <<
                            tr("Wine");
}

QString Drink::typeStringFromType(Drink::Type type)
{
    int typeIndex = static_cast<int>(type);
    typeIndex -= 1; // UnkownType
    QStringList types = typeStrings();
    if(typeIndex < 0 || typeIndex >= types.size())
        return tr("Unkown solo");

    return types.at(typeIndex);
}

Drink::Type Drink::typeFromString(const QString &typeString)
{
    int typeIndex = typeStrings().indexOf(typeString);
    if(typeIndex < 0)
        return Drink::UnkownType;

    typeIndex += 1; // UnkownType
    return static_cast<Drink::Type>(typeIndex);
}


QByteArray Drink::parseJSONData()
{
    QByteArray postData;
    postData.append("{");
    postData.append(QString("\"alcoholByVolume\"") + ":" + "\"" + QString::number(alcoholByVolume()) + "\"");
    postData.append(",");
    postData.append(QString("\"drinkType\"") + ":" + "\"" + typeString() + "\"");
    postData.append(",");
    postData.append(QString("\"name\"") + ":" + "\"" + name() + "\"");
    postData.append(",");
    postData.append(QString("\"volume\"") + ":" + "\"" + QString::number(volume()) + "\"");
    postData.append("}");

    return postData;
}

void Drink::parseUpdateFromJSON(QJsonObject object, bool created)
{
    Q_UNUSED(created)

    setAlcoholByVolume(object.value("alcoholByVolume").toString().toDouble());
    setType(typeFromString(object.value("drinkType").toString()));
    setName(object.value("name").toString());
    setVolume(object.value("volume").toString().toDouble());

    Qp::update(Qp::sharedFrom(this));
}

bool Drink::parseCheckAfterUploadConditions()
{
    QHash<QString, QSharedPointer<ParseObject>> hash = ParseObject::currentUploadingObjects;
    if(ParseObject::currentUploadingObjects.value("LiveDrink")) {
        QSharedPointer<ParseObject> liveDrink = ParseObject::currentUploadingObjects.value("LiveDrink");
        ParseObject::currentUploadingObjects.remove("LiveDrink");
        liveDrink->parseUpload();
    }

    return true;
}

