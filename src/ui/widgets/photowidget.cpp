#include "photowidget.h"
#include "ui_photowidget.h"

#include <QDir>

#include <data/game.h>
#include <library.h>
#include "photogamewidget.h"

PhotoWidget::PhotoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoWidget)
{
    ui->setupUi(this);
}

PhotoWidget::~PhotoWidget()
{
    delete ui;
}

void PhotoWidget::setGames(QList<QSharedPointer<Game> > games)
{
    cleanWidget();

    qSort(games.begin(), games.end(), sortGamesByDateLastFirst);

    m_games = games;

    QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setSpacing(0);

    foreach(QSharedPointer<Game> game, games) {
        QStringList files = checkForPhotos(game);
        if(files.isEmpty()) {
            continue;
        }

        PhotoGameWidget* gameWidget = new PhotoGameWidget(this);
        gameWidget->setGame(game, files);

        layout->addWidget(gameWidget);
    }

    ui->scrollAreaWidgetContents->setLayout(layout);
}

QList<QString> PhotoWidget::checkForPhotos(QSharedPointer<Game> game)
{
    QList<QString> list;

    QString package = Library::instance()->packagePath();
    QDir dir(package+"/photos/"+QString::number(Qp::primaryKey(game)));
    if(!dir.exists()){
        return list;
    }

    QStringList folders = dir.entryList(QDir::Dirs);
    foreach(QString folder, folders) {
        if(folder == "." || folder == "..") {
            continue;
        }
        dir.cd(folder);
        foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files)) {
            if(photoSuffix().contains(fileInfo.suffix())) {
                list.append(fileInfo.absoluteFilePath());
            }
        }
        dir.cdUp();
    }

    return list;
}

QStringList PhotoWidget::photoSuffix()
{
    return QStringList() << "jpg" << "png" << "JPG";
}

void PhotoWidget::cleanWidget()
{
    if (ui->scrollAreaWidgetContents->layout() != NULL )
    {
        QLayoutItem* item;
        while ( ( item =  ui->scrollAreaWidgetContents->layout()->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        delete  ui->scrollAreaWidgetContents->layout();
    }
}
