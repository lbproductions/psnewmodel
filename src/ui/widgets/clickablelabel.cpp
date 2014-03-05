#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) :
    QLabel(parent)
{
}


void ClickableLabel::mousePressEvent(QMouseEvent *ev)
{
    emit pressed();
    QLabel::mousePressEvent(ev);
}

void ClickableLabel::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit doubleClicked();
    QLabel::mouseDoubleClickEvent(e);
}
