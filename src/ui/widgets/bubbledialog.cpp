#include "bubbledialog.h"
#include "ui_bubbledialog.h"

BubbleDialog::BubbleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BubbleDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

BubbleDialog::~BubbleDialog()
{
    delete ui;
}
