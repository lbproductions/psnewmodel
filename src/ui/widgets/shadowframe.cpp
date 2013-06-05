#include "shadowframe.h"

#include <QVBoxLayout>
#include <QPainter>

ShadowFrame::ShadowFrame(QWidget *parent) :
    QWidget(parent),
    m_widget(nullptr),
    m_shadowLength(9)
{
    m_centralLayout = new QVBoxLayout(this);
    m_centralLayout->setContentsMargins(0,0,0,0);
    m_centralLayout->setSpacing(0);

    QLinearGradient shadowGradient(0,0,m_shadowLength,0);
    shadowGradient.setColorAt(0.0, QColor(0,0,0,80));
    shadowGradient.setColorAt(0.3, QColor(21,21,21,40));
    shadowGradient.setColorAt(1.0, QColor(0,0,0,0));
    m_shadowBrush = QBrush(shadowGradient);
}

QWidget *ShadowFrame::widget() const
{
    return m_widget;
}

void ShadowFrame::setWidget(QWidget *widget)
{
    if(m_widget)
        m_centralLayout->removeWidget(m_widget);

    m_widget = widget;
    m_centralLayout->addWidget(m_widget);
}

void ShadowFrame::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    painter.setBrush(m_shadowBrush);
    painter.setPen(QPen(QColor(0,0,0,0)));
    painter.drawRect(QRect(QPoint(0,0), QPoint(m_shadowLength, height())));
}

QBrush ShadowFrame::shadowBrush() const
{
    return m_shadowBrush;
}

void ShadowFrame::setShadowBrush(const QBrush &shadowBrush)
{
    m_shadowBrush = shadowBrush;
}
