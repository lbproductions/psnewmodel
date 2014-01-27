#include "dialogcontroller.h"

#include "gamewindow.h"

#include <QPropertyAnimation>
#include <QFrame>
#include <QHBoxLayout>

DialogController::DialogController(GameWindow *parent) :
    QObject(parent),
    m_gameWindow(parent),
    m_dialog(nullptr),
    m_dialogHeight(400),
    m_dialogOffsetTop(0),
    m_dialogOffsetLeft(29)
{
}

int DialogController::dialogHeight() const
{
    return m_dialogHeight;
}

void DialogController::setDialogHeight(int dialogHeight)
{
    m_dialogHeight = dialogHeight;
}

int DialogController::dialogOffsetTop() const
{
    return m_dialogOffsetTop;
}

void DialogController::setDialogOffsetTop(int dialogOffsetTop)
{
    m_dialogOffsetTop = dialogOffsetTop;
}

int DialogController::dialogOffsetLeft() const
{
    return m_dialogOffsetLeft;
}

void DialogController::setDialogOffsetLeft(int dialogOffsetLeft)
{
    m_dialogOffsetLeft = dialogOffsetLeft;
}

void DialogController::closeDialogOnMousePress(const QPoint &mousePos)
{
    if(!m_dialog)
        return;

    if(m_dialog->geometry().contains(mousePos))
        return;

    closeDialog();
    emit dialogClosed();
}

void DialogController::showDialog(QWidget *widget)
{
    widget = frameWidget(widget);
    widget->setFixedHeight(m_dialogHeight);
    widget->show();

    if(m_dialog) { // previous dialog
        m_dialog->close();
        m_dialog->deleteLater();
        m_dialog = widget;
        widget->setGeometry(visibleGeometry(widget));
        return; // only animate, if there was no dialog before.
    }

    m_dialog = widget;
    QPropertyAnimation *animation  = new QPropertyAnimation(m_dialog, "geometry");
    animation->setStartValue(hiddenGeometry(widget));
    animation->setEndValue(visibleGeometry(widget));
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->start();

    connect(animation, &QPropertyAnimation::finished,
            animation, &QPropertyAnimation::deleteLater);
}

void DialogController::closeDialog()
{
    QWidget *widget = m_dialog;
    m_dialog = nullptr;

    if(!widget)
        return;

    QRect geom = widget->geometry();
    int w = geom.width() + m_dialogOffsetLeft;
    QPropertyAnimation *animation  = new QPropertyAnimation(widget, "geometry");
    animation->setStartValue(geom);
    animation->setEndValue(geom.adjusted(-w,0,-w,0));
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->start();

    connect(animation, &QPropertyAnimation::finished, [=]{
        animation->deleteLater();
        widget->close();
        widget->deleteLater();
    });
}

QWidget *DialogController::frameWidget(QWidget *widget)
{
    QFrame *frame = new QFrame(m_gameWindow);
    frame->setLayout(new QHBoxLayout);
    frame->setObjectName("actionFrame");
    frame->setMinimumWidth(widget->width());
    frame->layout()->addWidget(widget);
    frame->layout()->setContentsMargins(0,0,0,0);
    frame->setStyleSheet("QFrame#actionFrame { background: rgb(35,35,35); border:none; border-right:3px solid rgb(108,108,108);}");
    return frame;
}

QRect DialogController::visibleGeometry(QWidget *widget) const
{
    QPoint topLeft = widget->mapFromGlobal(m_gameWindow->geometry().topLeft())
            + QPoint(m_dialogOffsetLeft, m_dialogOffsetTop);
    return QRect(topLeft, widget->size());
}

QRect DialogController::hiddenGeometry(QWidget *widget) const
{
    int w = widget->width() + m_dialogOffsetLeft;
    QRect geom = visibleGeometry(widget);
    return geom.adjusted(-w,0,-w,0);
}
