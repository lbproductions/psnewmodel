#include "scrollarea.h"

#include <QDebug>
#include <QScrollBar>

ScrollArea::ScrollArea(QWidget *parent) :
    QScrollArea(parent)
{
}

void ScrollArea::scrollContentsBy(int dx, int dy)
{
    QScrollArea::scrollContentsBy(dx,dy);
    adjustPositions();
}

void ScrollArea::adjustPositions()
{
    QPoint delta = QPoint(horizontalScrollBar()->value(),
                          verticalScrollBar()->value());

    foreach(QWidget *w, m_fixedWidgets) {
        w->move(delta);
    }
}

QList<QWidget *> ScrollArea::fixedWidgets() const
{
    return m_fixedWidgets;
}

void ScrollArea::setFixedWidgets(const QList<QWidget *> &fixedWidgets)
{
    m_fixedWidgets = fixedWidgets;
}

void ScrollArea::addFixedWidget(QWidget *widget)
{
    m_fixedWidgets.removeAll(widget);
    m_fixedWidgets.append(widget);
}

bool ScrollArea::event(QEvent *e)
{
    bool ret = QScrollArea::event(e);
    adjustPositions();
    return ret;
}

bool ScrollArea::eventFilter(QObject *o, QEvent *e)
{
    bool ret = QScrollArea::eventFilter(o, e);
    adjustPositions();
    return ret;
}

void ScrollArea::resizeEvent(QResizeEvent *e)
{
    QScrollArea::resizeEvent(e);
    adjustPositions();
}

void ScrollArea::setWidgetResizable(bool resizable)
{
    QScrollArea::setWidgetResizable(resizable);
    adjustPositions();
}
