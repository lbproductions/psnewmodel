#include "clickableimagelabel.h"

#include <QPainter>
#include <QImageReader>
#include <QElapsedTimer>
#include <QDebug>

ClickableImageLabel::ClickableImageLabel(QWidget *parent):
    ClickableLabel(parent),
    m_clicked(false),
    m_loaded(false)
{
    connect(this, SIGNAL(pressed()), this, SLOT(onClicked()));
}

void ClickableImageLabel::setImageFile(QString imageFile)
{
    m_imageFile = imageFile;
    m_loaded = false;
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

    if(!m_loaded) {
        loadImage();
    }

    if(m_image.isNull())
        return;

    //m_image = m_image.scaledToHeight(height());

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

void ClickableImageLabel::loadImage()
{
    //QElapsedTimer timer;
    //timer.start();

    m_loaded = true;

    if(m_imageFile.isEmpty()) {
        return;
    }

    QImageReader reader(m_imageFile);
    /*
    double ratio = (double)reader.size().width() / (double)reader.size().height();
    int widgetHeight = height();
    reader.setScaledSize(QSize(widgetHeight * ratio, widgetHeight));
    */
    reader.setScaledSize(QSize(width(), height()));
    m_image = reader.read();

    //qDebug() << "ClickableImageLabel::loadImage() took" << timer.elapsed() << "milliseconds";
}
