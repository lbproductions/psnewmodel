#ifndef OBJECTLISTWIDGET_H
#define OBJECTLISTWIDGET_H

#include <QListWidget>

class Object;

class ObjectListWidgetBase : public QListWidget
{
    Q_OBJECT
public:
    explicit ObjectListWidgetBase(QWidget *parent = 0);

signals:
    void objectActivated(QSharedPointer<QObject> object);

protected slots:
    virtual void indexActivated(const QModelIndex &index) = 0;

};

template<class O>
class ObjectListWidget : public ObjectListWidgetBase
{
public:
    explicit ObjectListWidget(QWidget *parent = 0);

    QList<QSharedPointer<O> > items() const;
    QSharedPointer<O> current() const;
    void setCurrent(QSharedPointer<O> object);

    bool contains(QSharedPointer<O> object) const;
    int indexOf(QSharedPointer<O> object) const;

    void add(const QList<QSharedPointer<O> > &items);
    void add(QSharedPointer<O> object);
    void remove(QSharedPointer<O> object);
    void clear();

protected:
    virtual QListWidgetItem *createItem(QSharedPointer<O> object) = 0;

private:
    void indexActivated(const QModelIndex &index) Q_DECL_OVERRIDE;

private:
    QList<QSharedPointer<O> > m_objects;
    QMultiHash<QSharedPointer<O>, int> m_indexes;
};

template<class O>
ObjectListWidget<O>::ObjectListWidget(QWidget *parent) :
    ObjectListWidgetBase(parent)
{
    connect(this, &QListWidget::activated,
            this, &ObjectListWidget<O>::indexActivated);
}

template<class O>
void ObjectListWidget<O>::clear()
{
    m_objects.clear();
    m_indexes.clear();
    QListWidget::clear();
}

template<class O>
QList<QSharedPointer<O> > ObjectListWidget<O>::items() const
{
    return m_objects;
}

template<class O>
QSharedPointer<O> ObjectListWidget<O>::current() const
{
    int index = currentIndex().row();
    if(index < 0 || index >= m_objects.size())
        return QSharedPointer<O>();

    return m_objects.at(index);
}

template<class O>
bool ObjectListWidget<O>::contains(QSharedPointer<O> object) const
{
    return m_indexes.contains(object);
}

template<class O>
int ObjectListWidget<O>::indexOf(QSharedPointer<O> object) const
{
    return m_indexes.value(object, -1);
}

template<class O>
void ObjectListWidget<O>::add(const QList<QSharedPointer<O> > &objects)
{
    foreach(QSharedPointer<O> p, objects)
        add(p);
}

template<class O>
void ObjectListWidget<O>::add(QSharedPointer<O> object)
{
    if(!object || contains(object))
        return;

    m_indexes.insert(object, m_objects.size());
    m_objects.append(object);
    QListWidgetItem *newItem = createItem(object);
    insertItem(m_objects.size(), newItem);
}

template<class O>
void ObjectListWidget<O>::setCurrent(QSharedPointer<O> object)
{
    if(!contains(object))
        return;

    int i = indexOf(object);
    scrollToItem(item(i));
    setCurrentRow(i);
}

template<class O>
void ObjectListWidget<O>::remove(QSharedPointer<O> object)
{
    while(contains(object)) {
        int index = indexOf(object);
        delete takeItem(index);
        m_indexes.remove(object, index);
    }

    m_indexes.clear();
    m_objects.removeAll(object);
    for(int i = 0; i < m_objects.size(); ++i) {
        m_indexes.insert(m_objects.at(i), i);
    }
}

template<class O>
void ObjectListWidget<O>::indexActivated(const QModelIndex &)
{
    QSharedPointer<O> object = current();
    if(object)
        emit objectActivated(qSharedPointerCast<QObject>(object));
}

#endif // OBJECTLISTWIDGET_H
