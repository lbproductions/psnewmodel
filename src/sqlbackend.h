#ifndef QPERSISTENCE_SQLBACKEND_H
#define QPERSISTENCE_SQLBACKEND_H

#include "defines.h"
BEGIN_CLANG_DIAGNOSTIC_IGNORE_WARNINGS
#include <QtCore/QExplicitlySharedDataPointer>
#include <QtCore/QObject>
#include <QtCore/QVariant>
END_CLANG_DIAGNOSTIC_IGNORE_WARNINGS

class QSqlDatabase;

class QpSqlBackend : public QObject
{
public:
    static QpSqlBackend *forDatabase(const QSqlDatabase &database);

    QpSqlBackend(QObject *parent);
    ~QpSqlBackend();

    virtual QString primaryKeyType() const = 0;
    virtual QString variantTypeToSqlType(QVariant::Type type) const = 0;
    virtual QString nowTimestamp() const = 0;
};

class QpSqliteBackend : public QpSqlBackend
{
public:
    QpSqliteBackend(QObject *parent);
    QString primaryKeyType() const Q_DECL_OVERRIDE;
    QString variantTypeToSqlType(QVariant::Type type) const Q_DECL_OVERRIDE;
    QString nowTimestamp() const;
};

class QpMySqlBackend : public QpSqlBackend
{
public:
    QpMySqlBackend(QObject *parent);
    QString primaryKeyType() const Q_DECL_OVERRIDE;
    QString variantTypeToSqlType(QVariant::Type type) const Q_DECL_OVERRIDE;
    QString nowTimestamp() const;
};

#endif // QPERSISTENCE_SQLBACKEND_H
