#include "popupwidget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QDialog>
#include <QKeyEvent>

PopupWidget::PopupWidget(QWidget *parent) :
    QFrame(parent),
    m_widget(nullptr),
    m_arrowPosition(Bottom),
    m_anchorWidget(nullptr)
{
    setLayout(new QHBoxLayout);
    setStyleSheet("PopupWidget {"
                  "border: 25px;"
                  "padding: -5px;"
                  "margin: 20px;"
                  "border-image: url(:/general/popup-border.png) 24 fill stretch;"
                  "background-color: rgb(51,51,51);"
                  "background-clip: content;"
                  "}");
    layout()->setContentsMargins(6,6,6,6);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setFocusPolicy(Qt::ClickFocus);
}

void PopupWidget::paintEvent(QPaintEvent *e)
{
    QFrame::paintEvent(e);

    QPainter painter(this);
    if(m_arrowPosition == Bottom) {
        painter.drawPixmap(width() / 2 - 53/2, height() - 39, 53, 32, QPixmap(":/general/popup-arrow.png"));
    }
    else if(m_arrowPosition == Left) {
        painter.drawPixmap(7, height() / 2 - 32 / 2, 32, 53, QPixmap(":/general/popup-arrow-left.png"));
    }
    else if(m_arrowPosition == Top) {
        painter.drawPixmap(width() / 2 - 53/2, 0, 53, 32, QPixmap(":/general/popup-arrow-top.png"));
    }
}

QWidget *PopupWidget::widget() const
{
    return m_widget;
}

void PopupWidget::setWidget(QWidget *widget)
{
    if(m_widget)
        layout()->removeWidget(m_widget);

    m_widget = widget;

    QDialog *dialog = qobject_cast<QDialog *>(widget);
    if(dialog) {
        connect(dialog, SIGNAL(finished(int)),
                this, SLOT(close()));
    }

    layout()->addWidget(m_widget);
}

PopupWidget::ArrowPosition PopupWidget::arrowPosition() const
{
    return m_arrowPosition;
}

void PopupWidget::setArrowPosition(const ArrowPosition &arrowPosition)
{
    m_arrowPosition = arrowPosition;
}

void PopupWidget::anchorTo(QWidget *widget)
{
    QPoint globalPos = widget->mapToGlobal(widget->mapFromParent(widget->pos()));
    QPoint myPos = mapFromGlobal(globalPos);

    if(m_arrowPosition == Bottom) {
        move(myPos.x() - minimumWidth() / 2 + widget->width() / 2,
             myPos.y() - minimumHeight() - widget->height() + 40);
    }
    else if(m_arrowPosition == Left) {
        move(myPos.x() + widget->width() - 25,
             myPos.y() - minimumHeight() / 2 + 8);
    }

    m_anchorWidget = widget;
}

void PopupWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape) {
        close();
    }
}

void PopupWidget::show()
{
    setFocus();
    QFrame::show();
}

void PopupWidget::updatePosition()
{
    if(!m_anchorWidget)
        return;

    anchorTo(m_anchorWidget);
}
