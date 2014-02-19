#include "imagewell.h"

#include <misc/tools.h>
#include <misc/settings.h>

#include <QMouseEvent>
#include <QFileDialog>
#include <QImageReader>
#include <QHBoxLayout>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColorDialog>

ImageWell::ImageWell(QWidget *parent) :
    QFrame(parent),
    m_type(PixmapWell),
    m_pixmapLabel(new QLabel(this))
{
    setLayout(new QHBoxLayout(this));
    layout()->addWidget(m_pixmapLabel);
    layout()->setContentsMargins(0,0,0,0);
    Tools::setStyleSheetFromResource(":/stylesheets/imagewell.qss", this);

    setPixmap(QPixmap());
}

QPixmap ImageWell::pixmap() const
{
    return m_pixmap;
}

void ImageWell::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    generatePixmaps(pixmap);
    m_pixmapLabel->setPixmap(m_displayPixmap);
}

void ImageWell::setPixmapFromFile(const QString &fileName)
{
    QPixmap pixmap;
    if(!pixmap.load(fileName) || pixmap.isNull())
        qWarning() << "Image could not be loaded: " << fileName;

    setPixmap(pixmap);
}

void ImageWell::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        QFrame::mouseReleaseEvent(event);

    if(m_type == PixmapWell) {
        QString file = QFileDialog::getOpenFileName(this,
                                     "Choose picture",
                                     GameSettings::openFileLocation(),
                                     SupportedFormats());
        if(!file.isEmpty())
            setPixmapFromFile(file);

        GameSettings::saveOpenFileLocation(file);
    }
    else if(m_type == ColorWell) {
        QColorDialog dialog;
        dialog.setCurrentColor(m_color);
        int ret = dialog.exec();
        if(ret == QDialog::Accepted)
            setColor(dialog.currentColor());
    }
}

void ImageWell::enterEvent(QEvent *)
{
    m_pixmapLabel->setPixmap(m_pixmapDarker);
}

void ImageWell::leaveEvent(QEvent *)
{
    m_pixmapLabel->setPixmap(m_displayPixmap);
}

QString ImageWell::SupportedFormats()
{
    QStringList list;
    foreach(QByteArray b, QImageReader::supportedImageFormats()) {
        QString ext = QString::fromLatin1(b);
        list.append(QString("*.%2").arg(ext.toLower()));
    }
    return QString("Images (%1)").arg(list.join(" "));
}

QColor ImageWell::color() const
{
    return m_color;
}

void ImageWell::setColor(const QColor &color)
{
    QPixmap pm(48,48);
    pm.fill(color);
    setPixmap(pm);

    m_color = color;
}

ImageWell::Type ImageWell::type() const
{
    return m_type;
}

void ImageWell::setType(const Type &type)
{
    m_type = type;
}

void ImageWell::generatePixmaps(const QPixmap pixmap)
{
    m_displayPixmap = pixmap;
    if(m_displayPixmap.isNull()) {
        m_displayPixmap = QPixmap(48,48);
        m_displayPixmap.fill(QColor(240,240,240));
    }
    m_displayPixmap = m_displayPixmap.scaledToHeight(48);
    if(m_displayPixmap.width() > 48)
        m_displayPixmap = m_displayPixmap.scaledToWidth(48);

    m_pixmapDarker = m_displayPixmap;
    QPainter p(&m_pixmapDarker);
    QBrush b(QColor(0,0,0,50));
    p.setBrush(b);
    QPen pen = p.pen();
    pen.setWidth(0);
    pen.setColor(QColor(0,0,0,0));
    p.setPen(pen);
    p.drawRect(0, 0, 48, 48);
}
