#ifndef TOOLS_H
#define TOOLS_H

#include <QPersistence.h>

#include <QAbstractItemView>
#include <QSortFilterProxyModel>

class Player;

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

    static QString progressBarStyleSheet(QColor color);

    static QString darkHeaderViewStyleSheet();

    static QVariant percentageString(double percentage, int precision = 2);
    static QVariant ifPositive(int number);
    static QVariant scaledPixmap(const QPixmap &pm, int h = 16, int w = 16);

    static QPalette darkPalette(QWidget* widget);

    static QVariant playersString(const QList<QSharedPointer<Player> > &players);

    static double percentage(double value1, double value2);

    static QPixmap colorListPixmap(int w, int h, const QList<QColor> colors);
    static QPixmap colorPixmap(int w, int h, const QColor &color);
    static QPixmap colorEllipse(int w, int h, const QColor &color);
    static QPixmap playersColorPixmap(QSharedPointer<Player> playerOne, QSharedPointer<Player> playerTwo, int w = 16, int h = 16);
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
    QpObjectListModel<T> *model = static_cast<QpObjectListModel<T> *>(
                qobject_cast<QpObjectListModelBase *>(view->model()));
    if(model)
        return model->objectByIndex(index);

    QpSortFilterProxyObjectModel<T> *model2 = static_cast<QpSortFilterProxyObjectModel<T> *>(
                qobject_cast<QpSortFilterProxyObjectModelBase *>(view->model()));
    if(model2)
        return model2->objectByIndex(index);

    Q_ASSERT_X(false, Q_FUNC_INFO,
               "This method may only be used with QpObjectListModel or QpSortFilterProxyObjectModel");
}

template<class T>
QList<QSharedPointer<T> > Tools::objectsFrom(QAbstractItemView *view)
{
    QpObjectListModelBase *model = static_cast<QpObjectListModelBase *>(view->model());
    int count = model->rowCount();

    QList<QSharedPointer<T> > result;
    for(int row = 0; row < count; ++row) {
        result.append(qSharedPointerCast<T>(model->objectByIndexBase(model->index(row))));
    }
    return result;
}

#endif // TOOLS_H
