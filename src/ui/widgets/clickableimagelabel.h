#ifndef CLICKABLEIMAGELABEL_H
#define CLICKABLEIMAGELABEL_H

#include "clickablelabel.h"

class ClickableImageLabel : public ClickableLabel
{
    Q_OBJECT
public:
    ClickableImageLabel(QWidget* parent = 0);

    void setImageFile(QString imageFile);

public slots:
    void onClicked();
    void deactivate();

private:
    void paintEvent(QPaintEvent *e);
    void loadImage();

    QImage m_image;
    QString m_imageFile;
    bool m_clicked;

    bool m_loaded;
};

#endif // CLICKABLEIMAGELABEL_H
