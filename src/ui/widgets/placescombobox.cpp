#include "placescombobox.h"

#include <data/place.h>

PlacesComboBox::PlacesComboBox(QWidget *parent) :
    QComboBox(parent)
{
}

void PlacesComboBox::clear()
{
    m_places.clear();
    m_indexes.clear();
    QComboBox::clear();
}

QList<QSharedPointer<Place> > PlacesComboBox::places() const
{
    return m_places;
}

QSharedPointer<Place> PlacesComboBox::currentPlace() const
{
    int index = currentIndex();
    if(index < 0 || index >= m_places.size())
        return QSharedPointer<Place>();

    return m_places.at(index);
}

bool PlacesComboBox::contains(QSharedPointer<Place> place) const
{
    return m_indexes.contains(place);
}

int PlacesComboBox::indexOf(QSharedPointer<Place> place) const
{
    return m_indexes.value(place, -1);
}

void PlacesComboBox::setPlaces(const QList<QSharedPointer<Place> > &places)
{
    clear();
    addPlaces(places);
}

void PlacesComboBox::addPlaces(const QList<QSharedPointer<Place> > &places)
{
    foreach(QSharedPointer<Place> p, places)
        addPlace(p);
}

void PlacesComboBox::addPlace(QSharedPointer<Place> place)
{
    if(!place || contains(place))
        return;

    m_places.append(place);
    m_indexes.insert(place, m_places.size());
    addItem(QIcon(place->cityEmblem()),
            place->displayString());
}

void PlacesComboBox::setCurrentPlace(QSharedPointer<Place> place)
{
    if(!contains(place))
        return;

    setCurrentIndex(indexOf(place));
}

void PlacesComboBox::removePlace(QSharedPointer<Place> place)
{
    while(contains(place)) {
        int index = indexOf(place);
        removeItem(index);
        m_indexes.remove(place, index);
    }

    m_places.removeAll(place);
}
