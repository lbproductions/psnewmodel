#ifndef CLICKABLEIMAGELABEL_H
#define CLICKABLEIMAGELABEL_H

#include "clickablelabel.h"

class ClickableImageLabel : public ClickableLabel
{
    Q_OBJECT
public:
    ClickableImageLabel(QWidget* parent = 0);

    void setImage(QImage image);

public slots:
    void onClicked();
    void deactivate();

private:
    void paintEvent(QPaintEvent *e);

    QImage m_image;
    bool m_clicked;
};

#endif // CLICKABLEIMAGELABEL_H
