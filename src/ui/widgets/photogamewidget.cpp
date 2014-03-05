#include "photogamewidget.h"
#include "ui_photogamewidget.h"

#include <data/game.h>
#include <data/round.h>
#include "photoroundwidget.h"


PhotoGameWidget::PhotoGameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoGameWidget)
{
    ui->setupUi(this);
}

PhotoGameWidget::~PhotoGameWidget()
{
    delete ui;
}

void PhotoGameWidget::setGame(QSharedPointer<Game> game, QHash<int, QStringList> files)
{
    if(!game) {
        return;
    }

    m_game = game;

    ui->labelGameName->setText(m_game->name());
    m_photoFiles = files;

    int count = 0;
    foreach(QStringList list, files.values()) {
        count += list.size();
    }

    if(count == 1) {
       ui->labelPhotoCount->setText(QString::number(count) + tr(" Photo"));
    }
    else {
        ui->labelPhotoCount->setText(QString::number(count) + tr(" Photos"));
    }

    QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);

    foreach(QSharedPointer<Round> round, m_game->rounds()) {
        if(files.keys().contains(round->number())) {
            PhotoRoundWidget* roundWidget = new PhotoRoundWidget(ui->scrollAreaWidgetContents);
            roundWidget->setRound(round, files.value(round->number()));
            layout->addWidget(roundWidget);
        }
    }

    ui->scrollAreaWidgetContents->setLayout(layout);

    /*

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
    */
}

void PhotoGameWidget::on_toolButtonBack_clicked()
{
    emit backButtonClicked();
}
