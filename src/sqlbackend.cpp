#include "sqlbackend.h"
#include <QSharedData>

#include "qpersistence.h"

#include <QHash>
#include <QSqlDatabase>

class QpSqlBackendData
{
public:
    static QHash<QString, QpSqlBackend *> backends;

    static QpSqlBackend *createForDatabase(const QSqlDatabase &database);
};
QHash<QString, QpSqlBackend *> QpSqlBackendData::backends;

QpSqlBackend *QpSqlBackendData::createForDatabase(const QSqlDatabase &database)
{
    static QObject GUARD;

    if(database.driverName() == QLatin1String("QSQLITE"))
        return new QpSqliteBackend(&GUARD);
    if(database.driverName() == QLatin1String("QMYSQL"))
        return new QpMySqlBackend(&GUARD);


    Q_ASSERT_X(false, Q_FUNC_INFO,
               QString("No backend for database '%1'")
               .arg(database.driverName())
               .toLatin1());

    return nullptr; // Never reached
}

QpSqlBackend *QpSqlBackend::forDatabase(const QSqlDatabase &database)
{
    auto it = QpSqlBackendData::backends.find(database.driverName());

    if(it != QpSqlBackendData::backends.end())
        return it.value();

    QpSqlBackend *backend = QpSqlBackendData::createForDatabase(database);
    QpSqlBackendData::backends.insert(database.driverName(),backend);
    return backend;
}

bool QpSqlBackend::hasFeature(QpSqlBackend::Feature feature)
{
    return (QpSqliteBackend::forDatabase(Qp::database())->features() & feature);
}

QpSqlBackend::QpSqlBackend(QObject *parent) :
    QObject(parent)
{
}

QpSqlBackend::~QpSqlBackend()
{
}

QpSqliteBackend::QpSqliteBackend(QObject *parent) :
    QpSqlBackend(parent)
{
}

QpSqlBackend::Features QpSqliteBackend::features() const
{
    return 0;
}

QString QpSqliteBackend::primaryKeyType() const
{
    return QLatin1String("INTEGER PRIMARY KEY AUTOINCREMENT");
}

QString QpSqliteBackend::uniqueKeyType() const
{
    return QLatin1String("UNIQUE");
}

QString QpSqliteBackend::variantTypeToSqlType(QVariant::Type type) const
{
    switch (type) {
    case QVariant::UInt:
    case QVariant::Int:
    case QVariant::Bool:
    case QVariant::ULongLong:
        return QLatin1String("INTEGER");
    case QVariant::String:
    case QVariant::StringList:
    case QVariant::Date:
    case QVariant::DateTime:
    case QVariant::Time:
    case QVariant::Char:
    case QVariant::Url:
        return QLatin1String("TEXT");
    case QVariant::Double:
        return QLatin1String("REAL");
    case QVariant::UserType:
    default:
        return QLatin1String("BLOB");
    }
}

QpMySqlBackend::QpMySqlBackend(QObject *parent) :
    QpSqlBackend(parent)
{
}

QpSqlBackend::Features QpMySqlBackend::features() const
{
    return QpSqliteBackend::LocksFeature
            | QpSqliteBackend::TimestampsFeature;
}

QString QpMySqlBackend::primaryKeyType() const
{
    return QLatin1String("INTEGER PRIMARY KEY AUTO_INCREMENT");
}

QString QpMySqlBackend::uniqueKeyType() const
{
    return QLatin1String("UNIQUE KEY");
}

QString QpMySqlBackend::variantTypeToSqlType(QVariant::Type type) const
{
    switch (type) {
    case QVariant::UInt:
    case QVariant::Int:
        return QLatin1String("INTEGER");
    case QVariant::Bool:
    case QVariant::ULongLong:
        return QLatin1String("BIGINT");
    case QVariant::String:
    case QVariant::StringList:
    case QVariant::Url:
        return QLatin1String("TEXT");
    case QVariant::Date:
        return QLatin1String("DATE");
    case QVariant::DateTime:
        return QLatin1String("DATETIME");
    case QVariant::Time:
        return QLatin1String("TIMESTAMP");
    case QVariant::Char:
        return QLatin1String("CHAR");
    case QVariant::Double:
        return QLatin1String("FLOAT");
    case QVariant::UserType:
    default:
        return QLatin1String("BLOB");
    }
}


QString QpSqliteBackend::nowTimestamp() const
{
    return QLatin1String("date('now')");
}

QString QpSqliteBackend::orIgnore() const
{
    return QLatin1String("OR IGNORE");
}

QString QpMySqlBackend::nowTimestamp() const
{
    return QLatin1String("NOW()");
}

QString QpMySqlBackend::orIgnore() const
{
    return QLatin1String("IGNORE");
}
