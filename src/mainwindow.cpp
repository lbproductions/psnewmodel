#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <model/playerslistmodel.h>

#include <QSortFilterProxyModel>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    PlayersListModel *playerModel = new PlayersListModel(this);
    QSortFilterProxyModel *model = new QSortFilterProxyModel(this);
    model->setSourceModel(playerModel);

    ui->treeViewPlayers->setModel(model);

    QTimer::singleShot(0, this, SLOT(showMaximized()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
