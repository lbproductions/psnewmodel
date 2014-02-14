#include "chooselibrarywidget.h"
#include "ui_chooselibrarywidget.h"

#include "mainwindow.h"

#include <library.h>
#include <misc/settings.h>

#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

ChooseLibraryWidget::ChooseLibraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseLibraryWidget),
    m_moving(false)
{
    ui->setupUi(this);

    ui->labelVersion->setText(tr("Version %1").arg(QApplication::instance()->applicationVersion()));
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    // TODO: Icons for ChooseLibraryWidget
    ui->labelIcon1->setVisible(false);
    ui->labelIcon1_2->setVisible(false);

    QSettings settings;
    ui->checkBoxAutoOpen->setChecked(settings.value("chooselibrarywidget/autoopenlibrary", true).toBool());

    connect(ui->toolButtonClose, &QToolButton::clicked, this, &QWidget::close);
}

ChooseLibraryWidget::~ChooseLibraryWidget()
{
    delete ui;
}

void ChooseLibraryWidget::showOrOpenLibrary()
{
    // Library from command line argument
    QString databaseFilePath = Library::fileNameFromArguments();
    if(!databaseFilePath.isEmpty()) {
        openLibrary(databaseFilePath);
        return;
    }

    QSettings settings;

    // Library from settings (opened at last start)
    if(settings.value("chooselibrarywidget/autoopenlibrary").toBool()) {
        databaseFilePath = Library::fileNameFromSettings();

        if(!databaseFilePath.isEmpty()) {
            openLibrary(databaseFilePath);
            return;
        }
    }

    show();
}

void ChooseLibraryWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    qreal opacity(0.675);
    int roundness(5);
    QRect widget_rect = this->rect();

    QPainter painter(this);
    painter.save();

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::red);

    // clip
    QPainterPath rounded_rect;
    rounded_rect.addRoundRect(widget_rect, roundness);
    painter.setClipPath(rounded_rect);

    // get clipping region
    QRegion maskregion = painter.clipRegion();

    // mask the widget
    setMask(maskregion);
    painter.setOpacity(opacity);

    // fill path with color
    painter.fillPath(rounded_rect,QBrush(Qt::black));

    // restore painter
    painter.restore();
}

void ChooseLibraryWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton) {
        m_moving = true;
        m_offset = event->pos();
    }
}

void ChooseLibraryWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if(m_moving)
        move(event->globalPos() - m_offset);
}

void ChooseLibraryWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton) {
        m_moving = false;
    }
}

void ChooseLibraryWidget::on_frameButton1_clicked()
{
    QString file = Library::fileNameLocal();
    Q_ASSERT(!file.isEmpty());
    openLibrary(file);
}

void ChooseLibraryWidget::on_frameButton1_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Create Library"), GameSettings::openFileLocation());

    if(fileName.isEmpty())
        return;

    QFile file(fileName);
    if(file.exists() && !file.remove()) {
        QMessageBox msg;
        msg.setText(tr("Could not remove file '%1'!")
                    .arg(fileName));
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
        QApplication::quit();
    }

    if(!Library::createFileIfNotExists(fileName))
        return;

    Library *library = Library::instance();
    library->setFileName("");

    GameSettings::saveOpenFileLocation(fileName);
    openLibrary(fileName);
}

void ChooseLibraryWidget::openLibrary(const QString &libraryPath)
{
    QSettings settings;

    Library *library = Library::instance();

    if(library->fileName() != libraryPath) {
        if(library->isOpen()) {
            Library::restartAndOpenLibrary(libraryPath);
            return;
        }
        else {
            library->setFileName(libraryPath);
            library->open();
        }
    }

    close();

    settings.setValue("chooselibrarywidget/autoopenlibrary", ui->checkBoxAutoOpen->isChecked());
    MainWindow *window = new MainWindow;
    window->show();
    window->raise();
    window->activateWindow();
}

void ChooseLibraryWidget::on_toolButtonOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Library"), GameSettings::openFileLocation());

    if(fileName.isEmpty())
        return;

    GameSettings::saveOpenFileLocation(fileName);
    openLibrary(fileName);
}
