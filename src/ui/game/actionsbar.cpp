#include "actionsbar.h"
#include "ui_actionsbar.h"

#include <QToolButton>
#include <QPropertyAnimation>

ActionsBar::ActionsBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ActionsBar),
    m_cardOffset(0),
    m_cardHeight(200),
    m_buttonsOffset(0),
    m_cardWidget(nullptr)
{
    ui->setupUi(this);
}

ActionsBar::~ActionsBar()
{
    delete ui;
}
int ActionsBar::topOffest() const
{
    return m_buttonsOffset;
}

void ActionsBar::setButtonsOffest(int topOffset)
{
    m_buttonsOffset = topOffset;
    ui->spacerWidget->setFixedHeight(topOffset);
}

void ActionsBar::addActionButton(QAction *action)
{
    QToolButton *button = new QToolButton(this);
    button->setIcon(action->icon());

    ui->verticalLayout->insertWidget(ui->verticalLayout->count() - 1, button);

    connect(button, &QToolButton::clicked,
            action, &QAction::trigger);
}

void ActionsBar::showCardWidget(QWidget *widget)
{
    closeCardWidget();

    m_cardWidget = widget;
    widget->setFixedHeight(m_cardHeight);

    int width = m_cardWidget->width();
    QPoint topRight = geometry().topRight() + QPoint(-5, m_cardOffset);

    QPropertyAnimation *animation  = new QPropertyAnimation(m_cardWidget, "geometry");
    animation->setStartValue(QRect(topRight - QPoint(width, 0),
                                   QSize(0, m_cardHeight)));
    animation->setEndValue(QRect(topRight,
                                  QSize(width, m_cardHeight)));
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->start();

    connect(animation, &QPropertyAnimation::finished,
            animation, &QPropertyAnimation::deleteLater);
    widget->stackUnder(this);
    widget->show();
}

void ActionsBar::closeCardWidget()
{
    QWidget * widget = m_cardWidget;
    m_cardWidget = nullptr;

    if(!widget)
        return;

    widget->setFixedHeight(m_cardHeight);

    int width = widget->width();
    QPoint topRight = geometry().topRight() + QPoint(0, m_cardOffset);

    QPropertyAnimation *animation  = new QPropertyAnimation(widget, "geometry");
    animation->setEndValue(QRect(topRight - QPoint(width, 0),
                                   QSize(0, m_cardHeight)));
    animation->setStartValue(QRect(topRight,
                                  QSize(width, m_cardHeight)));
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->start();

    connect(animation, &QPropertyAnimation::finished, [=]{
        animation->deleteLater();
        widget->close();
        widget->deleteLater();
    });

}
int ActionsBar::cardHeight() const
{
    return m_cardHeight;
}

void ActionsBar::setCardHeight(int cardHeight)
{
    m_cardHeight = cardHeight;
}
int ActionsBar::cardOffset() const
{
    return m_cardOffset;
}

void ActionsBar::setCardOffset(int cardOffset)
{
    m_cardOffset = cardOffset;
}


