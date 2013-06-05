#ifndef TOOLS_H
#define TOOLS_H

#include "../lib/QPersistence/src/abstractobjectlistmodel.h"

#include <QAbstractItemView>

class Tools
{
public:
    template<class T>
    static QSharedPointer<T> selectedObjectFrom(QAbstractItemView *view);

    static void setStyleSheetFromResource(const QString &resource, QWidget *widget, const QString &additionalStyles = QString());
};

template<class T>
QSharedPointer<T> Tools::selectedObjectFrom(QAbstractItemView *view)
{
    QModelIndexList list = view->selectionModel()->selectedIndexes();
    if(list.isEmpty())
        return QSharedPointer<T>();

    return qSharedPointerCast<T>(static_cast<QpAbstractObjectListModelBase *>(view->model())->objectByIndexBase(list.first()));
}

#endif // TOOLS_H
