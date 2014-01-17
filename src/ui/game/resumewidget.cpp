#include "resumewidget.h"
#include "ui_resumewidget.h"

ResumeWidget::ResumeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResumeWidget)
{
    ui->setupUi(this);

    this->setAutoFillBackground(true);
    QPalette palette(this->palette());
    QColor color(51,51,51);
    color.setAlpha(100);
    palette.setColor(QPalette::Background, color);
    this->setPalette(palette);
}

ResumeWidget::~ResumeWidget()
{
    delete ui;
}

void ResumeWidget::mousePressEvent(QMouseEvent */*e*/)
{
    this->setVisible(false);

    emit widgetClicked();
}
