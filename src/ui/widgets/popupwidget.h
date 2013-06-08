#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QFrame>

class PopupWidget : public QFrame
{
    Q_OBJECT
public:
    enum ArrowPosition {
        Left,
        Bottom
    };

    explicit PopupWidget(QWidget *parent = 0);

    void paintEvent(QPaintEvent *e);

    QWidget *widget() const;
    void setWidget(QWidget *widget);

    ArrowPosition arrowPosition() const;
    void setArrowPosition(const ArrowPosition &arrowPosition);

    void anchorTo(QWidget *widget);

    void keyPressEvent(QKeyEvent *e);
    void show();

public slots:
    void updatePosition();

private:
    QWidget *m_widget;
    ArrowPosition m_arrowPosition;
    QWidget *m_anchorWidget;
    
};

#endif // POPUPWIDGET_H
