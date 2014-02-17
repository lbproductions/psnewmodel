#include "startwidget.h"
#include "ui_startwidget.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"

StartWidget::StartWidget(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget)
{
    ui->setupUi(this);

    ui->treeViewGames->setAttribute(Qt::WA_MacShowFocusRect, false);
}

StartWidget::~StartWidget()
{
    delete ui;
}

void StartWidget::init(MainWindow *window)
{
    ui->toolButtonStartGame->setDefaultAction(window->ui->actionNew_Game);
}
