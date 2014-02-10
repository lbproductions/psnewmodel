#include "chooselibrarywidget.h"
#include "ui_chooselibrarywidget.h"

#include <QApplication>
#include <QPainter>
#include <QMouseEvent>

ChooseLibraryWidget::ChooseLibraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseLibraryWidget),
    m_moving(false)
{
    ui->setupUi(this);

    ui->labelVersion->setText(tr("Version %1").arg(QApplication::instance()->applicationVersion()));
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->toolButtonClose, &QToolButton::clicked, this, &QWidget::close);
}

ChooseLibraryWidget::~ChooseLibraryWidget()
{
    delete ui;
}

void ChooseLibraryWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    qreal opacity(0.675);
    int roundness(5);
    QRect widget_rect = this->rect();

    QPainter painter(this);
    painter.save();

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::red);

    // clip
    QPainterPath rounded_rect;
    rounded_rect.addRoundRect(widget_rect, roundness);
    painter.setClipPath(rounded_rect);

    // get clipping region
    QRegion maskregion = painter.clipRegion();

    // mask the widget
    setMask(maskregion);
    painter.setOpacity(opacity);

    // fill path with color
    painter.fillPath(rounded_rect,QBrush(Qt::black));

    // restore painter
    painter.restore();
}

void ChooseLibraryWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton) {
        m_moving = true;
        m_offset = event->pos();
    }
}

void ChooseLibraryWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if(m_moving)
        move(event->globalPos() - m_offset);
}

void ChooseLibraryWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton) {
        m_moving = false;
    }
}
