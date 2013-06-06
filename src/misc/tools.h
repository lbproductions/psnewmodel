#ifndef TOOLS_H
#define TOOLS_H

#include "../lib/QPersistence/src/abstractobjectlistmodel.h"

#include <QAbstractItemView>

class Tools
{
public:
    template<class T>
    static QSharedPointer<T> selectedObjectFrom(QAbstractItemView *view);
    template<class T>
    static QSharedPointer<T> objectFrom(const QModelIndex &index, QAbstractItemView *view);
    template<class T>
    static QList<QSharedPointer<T> > objectsFrom(QAbstractItemView *view);


    static void setStyleSheetFromResource(const QString &resource, QWidget *widget, const QString &additionalStyles = QString());

    static QVariant percentageString(double percentage);
    static QVariant ifPositive(int number);
    static QVariant scaledPixmap(const QPixmap &pm, int h = 16, int w = 16);
};

template<class T>
QSharedPointer<T> Tools::selectedObjectFrom(QAbstractItemView *view)
{
    QModelIndexList list = view->selectionModel()->selectedIndexes();
    if(list.isEmpty())
        return QSharedPointer<T>();

    return objectFrom<T>(list.first(), view);
}

template<class T>
QSharedPointer<T> Tools::objectFrom(const QModelIndex &index, QAbstractItemView *view)
{
    return qSharedPointerCast<T>(static_cast<QpAbstractObjectListModelBase *>(view->model())->objectByIndexBase(index));
}

template<class T>
QList<QSharedPointer<T> > Tools::objectsFrom(QAbstractItemView *view)
{
    QpAbstractObjectListModelBase *model = static_cast<QpAbstractObjectListModelBase *>(view->model());
    int count = model->rowCount();

    QList<QSharedPointer<T> > result;
    for(int row = 0; row < count; ++row) {
        result.append(qSharedPointerCast<T>(model->objectByIndexBase(model->index(row))));
    }
    return result;
}

#endif // TOOLS_H
