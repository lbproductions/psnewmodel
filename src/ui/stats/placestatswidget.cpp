#include "placestatswidget.h"
#include "ui_placestatswidget.h"

PlaceStatsWidget::PlaceStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaceStatsWidget)
{
    ui->setupUi(this);
}

PlaceStatsWidget::~PlaceStatsWidget()
{
    delete ui;
}
