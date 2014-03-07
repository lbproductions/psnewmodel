#include "photopreviewwidget.h"
#include "ui_photopreviewwidget.h"#

#include <data/game.h>


PhotoPreviewWidget::PhotoPreviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoPreviewWidget)
{
    ui->setupUi(this);

    connect(ui->imageLabel, SIGNAL(doubleClicked()), this, SIGNAL(doubleClicked()));
    connect(ui->imageLabel, SIGNAL(pressed()), this, SLOT(onImageClicked()));
}

PhotoPreviewWidget::~PhotoPreviewWidget()
{
    delete ui;
}

void PhotoPreviewWidget::setPreviewFile(QString file)
{
    ui->imageLabel->setImageFile(file);
}

void PhotoPreviewWidget::setGame(QSharedPointer<Game> game)
{
    m_game = game;

    ui->labelText->setText(game->name());
}

QSharedPointer<Game> PhotoPreviewWidget::game()
{
    return m_game;
}

void PhotoPreviewWidget::onImageClicked()
{
    emit imageClicked(ui->imageLabel);
}
