#include "photogamewidget.h"
#include "ui_photogamewidget.h"

#include <data/game.h>


PhotoGameWidget::PhotoGameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoGameWidget)
{
    ui->setupUi(this);

    //ui->framePhotos->setVisible(false);
}

PhotoGameWidget::~PhotoGameWidget()
{
    delete ui;
}

void PhotoGameWidget::setGame(QSharedPointer<Game> game, QStringList files)
{
    if(!game) {
        return;
    }

    m_game = game;

    ui->labelGameName->setText(m_game->name());

    m_photoFiles = files;

    if(m_photoFiles.size() == 1) {
       ui->labelPhotoCount->setText(QString::number(m_photoFiles.size()) + tr(" Photo"));
    }
    else {
        ui->labelPhotoCount->setText(QString::number(m_photoFiles.size()) + tr(" Photos"));
    }

    QGridLayout* layout = new QGridLayout(ui->framePhotos);
    int count = 0;
    foreach(QString file, m_photoFiles) {
        QImage image(file);
        if(image.isNull())
            continue;

        QLabel* imageLabel = new QLabel(ui->framePhotos);
        imageLabel->setPixmap(QPixmap::fromImage(image).scaledToHeight(200));

        layout->addWidget(imageLabel, count / 3, count % 3);

        count++;
    }

    ui->framePhotos->setLayout(layout);

}

void PhotoGameWidget::on_toolButtonShowPhotos_clicked()
{
    ui->framePhotos->setVisible(ui->framePhotos->isVisible());
}
