#include "clickableimagelabel.h"

#include <QPainter>

ClickableImageLabel::ClickableImageLabel(QWidget *parent):
    ClickableLabel(parent),
    m_clicked(false)
{
    connect(this, SIGNAL(pressed()), this, SLOT(onClicked()));
}

void ClickableImageLabel::setImage(QImage image)
{
    m_image = image;
}

void ClickableImageLabel::onClicked()
{
    m_clicked = true;
    repaint();
}

void ClickableImageLabel::deactivate()
{
    m_clicked = false;
    repaint();
}

void ClickableImageLabel::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    if(m_image.isNull())
        return;

    m_image = m_image.scaledToHeight(height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(m_clicked) {
        QPen pen = painter.pen();
        pen.setWidth(3);
        pen.setColor(QColor(102, 188, 34));
        painter.setPen(pen);
    }

    painter.setBrush(QBrush(m_image));
    painter.drawRoundedRect(3, 3, width()-6, height()-6, 5, 5);
}
