#ifndef IMAGEWELL_H
#define IMAGEWELL_H

#include <QFrame>

#include <QLabel>
#include <QPixmap>

class ImageWell : public QFrame
{
    Q_OBJECT
public:
    enum Type {
        PixmapWell,
        ColorWell
    };

    explicit ImageWell(QWidget *parent = 0);

    QPixmap pixmap() const;
    void setPixmap(const QPixmap &pixmap);

    void setPixmapFromFile(const QString &fileName);

    void mouseReleaseEvent(QMouseEvent *event);

    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

    QColor color() const;
    void setColor(const QColor &color);

    Type type() const;
    void setType(const Type &type);

private:
    QPixmap m_pixmap;
    QPixmap m_pixmapDarker;
    QPixmap m_displayPixmap;

    QColor m_color;

    Type m_type;

    void generatePixmaps(const QPixmap pixmap);

    QLabel *m_pixmapLabel;
    static QString SupportedFormats();
};

#endif // IMAGEWELL_H
