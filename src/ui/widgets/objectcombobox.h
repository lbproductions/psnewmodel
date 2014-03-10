#ifndef OBJECTCOMBOBOX_H
#define OBJECTCOMBOBOX_H

#include <QComboBox>
#include <QDebug>

class ObjectComboBoxBase : public QComboBox
{
    Q_OBJECT
public:
    explicit ObjectComboBoxBase(QWidget *parent = 0);

signals:
    void currentObjectChanged();
};

template<class T>
class ObjectComboBox : public ObjectComboBoxBase
{
public:
    explicit ObjectComboBox(QWidget *parent = 0);

    QSharedPointer<T> currentObject();

    void setObjects(QList<QSharedPointer<T> > objects);

    bool containsObject(QSharedPointer<T> object);

    void addObject(QSharedPointer<T> object);

    void removeObject(QSharedPointer<T> object);

    void setCurrentObject(QSharedPointer<T> object);

    void clear();

private:
    QList<QSharedPointer<T> > m_objects;
};

template<class T>
ObjectComboBox<T>::ObjectComboBox(QWidget *parent):
    ObjectComboBoxBase(parent)
{
}

template<class T>
QSharedPointer<T> ObjectComboBox<T>::currentObject()
{
    int index = this->currentIndex();
    if(index < 1 || index > m_objects.size()) {
        return QSharedPointer<T>();
    }

    return m_objects.at(index-1);
}

template<class T>
void ObjectComboBox<T>::setObjects(QList<QSharedPointer<T> > objects)
{
    this->clear();

    m_objects = objects;

    this->addItem("---");
    foreach(QSharedPointer<T> object, objects) {
        if(object) {
            this->addItem(object->name());
        }
    }
}

template<class T>
bool ObjectComboBox<T>::containsObject(QSharedPointer<T> object)
{
    return m_objects.contains(object);
}

template<class T>
void ObjectComboBox<T>::addObject(QSharedPointer<T> object)
{
    if(m_objects.contains(object))
        return;

    m_objects.append(object);
    this->addItem(object->name());
}

template<class T>
void ObjectComboBox<T>::removeObject(QSharedPointer<T> object)
{
    if(!m_objects.contains(object))
        return;

    this->removeItem(m_objects.indexOf(object)+1);
    m_objects.removeOne(object);
}

template<class T>
void ObjectComboBox<T>::setCurrentObject(QSharedPointer<T> object)
{
    if(!m_objects.contains(object))
        return;

    this->setCurrentIndex(m_objects.indexOf(object)+1);
}

template<class T>
void ObjectComboBox<T>::clear()
{
    m_objects.clear();
    QComboBox::clear();
}

class Game;
class Player;
class Place;
class Round;

typedef ObjectComboBox<Game> GameComboBox;
typedef ObjectComboBox<Player> PlayerComboBox;
typedef ObjectComboBox<Place> PlaceComboBox;
typedef ObjectComboBox<Round> RoundComboBox;

#endif // OBJECTCOMBOBOX_H
