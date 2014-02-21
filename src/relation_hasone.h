#ifndef RELATION_HASONE_H
#define RELATION_HASONE_H

#include <QExplicitlySharedDataPointer>
#include <QSharedPointer>
#include <QVariant>
#include <QMetaMethod>

class QpHasOneData;

class QpHasOneBase
{
public:
    explicit QpHasOneBase(const QString &name, QObject *parent);
    virtual ~QpHasOneBase();

    QSharedPointer<QObject> object() const;
    void setObject(const QSharedPointer<QObject> object) const;

private:
    QExplicitlySharedDataPointer<QpHasOneData> data;
    Q_DISABLE_COPY(QpHasOneBase)
};

template<class T>
class QpHasOne : public QpHasOneBase
{
public:
    explicit QpHasOne(const QString &name, QObject *parent) : QpHasOneBase(name, parent) {}
    virtual ~QpHasOne() {}

    operator QSharedPointer<T> () const { return qSharedPointerCast<T>(object()); }
    QpHasOne &operator=(const QSharedPointer<T> object) { setObject(qSharedPointerCast<QObject>(object)); return *this; }
};

#ifndef QpRelation
#define QpRelation(Address) \
    "" #Address, this
#endif

#endif // RELATION_HASONE_H
