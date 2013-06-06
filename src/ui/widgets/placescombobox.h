#ifndef PLACESCOMBOBOX_H
#define PLACESCOMBOBOX_H

#include <QComboBox>

class Place;

class PlacesComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit PlacesComboBox(QWidget *parent = 0);

    void clear();

    QList<QSharedPointer<Place> > places() const;
    QSharedPointer<Place> currentPlace() const;
    bool contains(QSharedPointer<Place> place) const;

    int indexOf(QSharedPointer<Place> place) const;

    void setPlaces(const QList<QSharedPointer<Place> > &places);
    void addPlaces(const QList<QSharedPointer<Place> > &places);
    void addPlace(QSharedPointer<Place> place);
    void setCurrentPlace(QSharedPointer<Place> place);

    void removePlace(QSharedPointer<Place> place);

private:
    QList<QSharedPointer<Place> > m_places;
    QMultiHash<QSharedPointer<Place>, int> m_indexes;
};
#endif // PLACESCOMBOBOX_H
