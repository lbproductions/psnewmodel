#ifndef SHADOWFRAME_H
#define SHADOWFRAME_H

#include <QWidget>

class ShadowFrame : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowFrame(QWidget *parent = 0);

    QWidget *widget() const;
    void setWidget(QWidget *widget);

    void paintEvent(QPaintEvent *e);

    QBrush shadowBrush() const;
    void setShadowBrush(const QBrush &shadowBrush);

private:
    QWidget *m_widget;
    QLayout *m_centralLayout;

    QBrush m_shadowBrush;
    int m_shadowLength;
};

#endif // SHADOWFRAME_H
