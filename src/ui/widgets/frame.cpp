#include "frame.h"

#include <QMouseEvent>

Frame::Frame(QWidget *parent) :
    QFrame(parent)
{
}

void Frame::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        emit clicked();
    }

    QFrame::mouseReleaseEvent(e);
}
