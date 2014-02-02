#include "dialogcontroller.h"

#include "gamewindow.h"

#include <QPropertyAnimation>
#include <QFrame>
#include <QHBoxLayout>

DialogController::DialogController(GameWindow *parent) :
    QObject(parent),
    m_gameWindow(parent),
    m_dialog(nullptr),
    m_widget(nullptr),
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
}

void DialogController::showDialog(QWidget *widget)
{
    QWidget *frame = frameWidget(widget);
    frame->setFixedHeight(m_dialogHeight);
    frame->show();

    if(m_dialog) { // previous dialog
        if(m_widget)
            disconnect(m_widget, 0, this, 0);

        m_dialog->close();
        m_dialog->deleteLater();
        m_widget = widget;
        m_dialog = frame;
        frame->setGeometry(visibleGeometry(frame));
        return; // only animate, if there was no dialog before.
    }

    m_widget = widget;
    m_dialog = frame;
    connect(widget, &QWidget::destroyed, this, &DialogController::closeDialogWhenDestroyed);

    QPropertyAnimation *animation  = new QPropertyAnimation(m_dialog, "geometry");
    animation->setStartValue(hiddenGeometry(frame));
    animation->setEndValue(visibleGeometry(frame));
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->start();

    connect(animation, &QPropertyAnimation::finished,
            animation, &QPropertyAnimation::deleteLater);
}

void DialogController::closeDialog()
{
    QWidget *dialog = m_dialog;
    m_dialog = nullptr;
    m_widget = nullptr;

    if(!dialog)
        return;

    QRect geom = dialog->geometry();
    int w = geom.width() + m_dialogOffsetLeft;
    QPropertyAnimation *animation  = new QPropertyAnimation(dialog, "geometry");
    animation->setStartValue(geom);
    animation->setEndValue(geom.adjusted(-w,0,-w,0));
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->start();

    connect(animation, &QPropertyAnimation::finished, [=]{
        animation->deleteLater();
        dialog->close();
        dialog->deleteLater();
    });

    emit dialogClosed();
}

void DialogController::closeDialogWhenDestroyed()
{
    QWidget *w = static_cast<QWidget*>(sender());
    if(w != m_widget)
        return;

    closeDialog();
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
