#include "photowidget.h"
#include "ui_photowidget.h"

#include <QDir>
#include <QLabel>

#include <data/game.h>
#include <library.h>
#include <misc/tools.h>
#include "photogamewidget.h"
#include "photopreviewwidget.h"
#include "clickableimagelabel.h"
#include <ui/mainwindow.h>


PhotoWidget::PhotoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoWidget),
    m_selectedLabel(0)
{
    ui->setupUi(this);

    depth = 0;
}

PhotoWidget::~PhotoWidget()
{
    delete ui;
}

void PhotoWidget::setGames(QList<QSharedPointer<Game> > games)
{
    disconnect(m_mainWindow, SIGNAL(photosAdded()), this, SLOT(onPhotoAdded()));

    Tools::clearWidget(ui->scrollAreaWidgetContents);

    qSort(games.begin(), games.end(), sortGamesByDateLastFirst);

    QVBoxLayout* outerLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);

    QHBoxLayout* innerLayout = new QHBoxLayout(ui->scrollAreaWidgetContents);

    QGridLayout* layout = new QGridLayout(ui->scrollAreaWidgetContents);
    layout->setSpacing(0);

    int count = 0;
    foreach(QSharedPointer<Game> game, games) {
        QHash<int, QStringList> files = checkForPhotos(game);
        if(files.isEmpty()) {
            continue;
        }

        QImage image(files.values().first().first());
        if(image.isNull())
            continue;

        PhotoPreviewWidget* previewWidget = new PhotoPreviewWidget(this);
        previewWidget->setPreviewFile(files.values().first().first());
        previewWidget->setGame(game);
        connect(previewWidget, SIGNAL(imageClicked(ClickableImageLabel*)), this, SLOT(onPreviewImageClicked(ClickableImageLabel*)));
        connect(previewWidget, SIGNAL(doubleClicked()), this, SLOT(onPreviewWidgetDoubleClicked()));
        m_previewWidgets.append(previewWidget);

        layout->addWidget(previewWidget, count / 7 , count %7);

        m_games.insert(game, files);

        count++;
    }

    innerLayout->addLayout(layout);
    innerLayout->addStretch();

    outerLayout->addLayout(innerLayout);
    outerLayout->addStretch();

    ui->scrollAreaWidgetContents->setLayout(outerLayout);

    connect(m_mainWindow, SIGNAL(photosAdded()), this, SLOT(onPhotoAdded()));
}

void PhotoWidget::setMainWindow(MainWindow *mainWindow)
{
    if(!mainWindow)
        return;

    m_mainWindow = mainWindow;
}

void PhotoWidget::onPreviewImageClicked(ClickableImageLabel *label)
{
    if(m_selectedLabel && m_selectedLabel != label) {
        m_selectedLabel->deactivate();
    }

    m_selectedLabel = label;
}

void PhotoWidget::onPreviewWidgetDoubleClicked()
{
    PhotoPreviewWidget* previewWidget = static_cast<PhotoPreviewWidget*>(sender());

    Tools::clearWidget(ui->page_2);

    QVBoxLayout* layout = new QVBoxLayout(ui->page_2);
    layout->setContentsMargins(0,0,0,0);

    PhotoGameWidget* gameWidget = new PhotoGameWidget(this);
    gameWidget->setGame(previewWidget->game(), m_games.value(previewWidget->game()));
    connect(gameWidget, SIGNAL(backButtonClicked()), this, SLOT(onBackButtonClicked()));

    layout->addWidget(gameWidget);

    ui->page_2->setLayout(layout);

    ui->stackedWidget->setCurrentIndex(1);
}

void PhotoWidget::onBackButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void PhotoWidget::onPhotoAdded()
{
    setGames(Qp::readAll<Game>());
}

QHash<int, QStringList> PhotoWidget::checkForPhotos(QSharedPointer<Game> game)
{
    QHash<int, QStringList> hash;

    QString package = Library::instance()->packagePath();
    QDir dir(package+"/photos/"+QString::number(Qp::primaryKey(game)));
    if(!dir.exists()){
        return hash;
    }

    QStringList folders = dir.entryList(QDir::Dirs);
    foreach(QString folder, folders) {
        if(folder == "." || folder == "..") {
            continue;
        }

        QStringList list;

        dir.cd(folder);
        foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files)) {
            if(photoSuffix().contains(fileInfo.suffix())) {
                list.append(fileInfo.absoluteFilePath());
            }
        }
        dir.cdUp();

        bool convertOk;
        int roundNumber = folder.toInt(&convertOk);
        if(!convertOk) {
            roundNumber = -1;
        }

        if(!list.isEmpty()) {
            hash.insert(roundNumber, list);
        }
    }

    return hash;
}

QStringList PhotoWidget::photoSuffix()
{
    return QStringList() << "jpg" << "png" << "JPG";
}
