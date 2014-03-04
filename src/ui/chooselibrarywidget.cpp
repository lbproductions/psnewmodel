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
#include <QStyledItemDelegate>
#include <QStandardPaths>

class RecentLibraryItemDelegate : public QStyledItemDelegate
{
public:
    RecentLibraryItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
    {}

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        QSize size = QStyledItemDelegate::sizeHint(option, index);
        size.setHeight(48);
        size.setWidth(option.widget->width());
        return size;
    }

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const Q_DECL_OVERRIDE
    {
        static QDir dir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
        static QString local = Library::fileNameLocal();

        if(option.state & QStyle::State_Selected) {
            painter->save();
            painter->setPen(Qt::transparent);
            painter->setBrush(option.palette.highlight());
            painter->drawRect(option.rect);
            painter->restore();
        }

        painter->save();

        QString filePath = index.data().toString();
        filePath = filePath.remove("/database/"+Library::instance()->defaultFileName());
        QFileInfo fileInfo(filePath);
        QString name = fileInfo.baseName();
        QString path = fileInfo.path();
        path.replace(dir.path(), tr("~"));

        if(filePath == local) {
            name = tr("Local");
            path = tr("Just play!");
        }

        QRect rect = option.rect;
        rect.adjust(8,5,-5,-5);
        QTextOption to;
        to.setWrapMode(QTextOption::NoWrap);
        painter->drawText(rect, name, to);

        painter->setPen(Qt::gray);
        to.setAlignment(Qt::AlignBottom | Qt::AlignLeft);
        painter->drawText(rect, path, to);
        painter->restore();

        // TODO: Library icons
    }
};


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

    ui->listWidgetRecent->setItemDelegate(new RecentLibraryItemDelegate(this));
    ui->listWidgetRecent->setAttribute(Qt::WA_MacShowFocusRect, false);

    m_recentLibraries = settings.value("chooselibrarywidget/recentlibraries").toStringList();
    foreach(QString path, m_recentLibraries) {
        QFile file(path);

        if(!file.exists())
            m_recentLibraries.removeAll(path);
        else
            ui->listWidgetRecent->addItem(path);
    }

    settings.setValue("chooselibrarywidget/recentlibraries", m_recentLibraries);

    if(!m_recentLibraries.isEmpty())
        ui->stackedWidget->setCurrentWidget(ui->pageList);

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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Create Package"), GameSettings::openFileLocation());
    qDebug() << "Try to create package: " << fileName;

    if(fileName.isEmpty())
        return;

    if(!fileName.endsWith(".pspkg")) {
        fileName = fileName + ".pspkg";
    }

    QDir dir(fileName);
    if(dir.exists()) {
        dir.removeRecursively();
    }
    else {
        QDir packageDir(QFileInfo(fileName).absolutePath());
        packageDir.mkdir(QFileInfo(fileName).fileName());
        packageDir.cd(QFileInfo(fileName).fileName());
        packageDir.mkdir("database");
        packageDir.cd("database");
    }

    if(!Library::createFileIfNotExists(fileName+"/database/"+Library::instance()->defaultFileName()))
        return;

    Library *library = Library::instance();
    library->setFileName("");

    GameSettings::saveOpenFileLocation(fileName);
    openLibrary(fileName);
}

void ChooseLibraryWidget::openLibrary(const QString &packagePath)
{
    qDebug() << "Try to open library in package: " << packagePath;

    QString libraryPath = packagePath+"/database/database.psdb";
    if(!QFile(libraryPath).exists()) {
        qWarning() << "Database could not be found in " << libraryPath;
        QApplication::quit();
        return;
    }

    QSettings settings;

    m_recentLibraries.removeAll(libraryPath);
    m_recentLibraries.prepend(libraryPath);
    settings.setValue("chooselibrarywidget/recentlibraries", m_recentLibraries);

    Library *library = Library::instance();
    if(library->fileName() != libraryPath) {
        if(library->isOpen()) {
            Library::restartAndOpenLibrary(libraryPath);
            return;
        }
        else {
            library->setFileName(libraryPath);
            library->setPackagePath(packagePath);
            if(!library->open()) {
                QApplication::quit();
                return;
            }
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Library"), GameSettings::openFileLocation(),
                                                    tr("ProjectStats Package (*.pspkg)"));

    if(fileName.isEmpty())
        return;

    GameSettings::saveOpenFileLocation(fileName);
    openLibrary(fileName);
}

void ChooseLibraryWidget::on_listWidgetRecent_activated(const QModelIndex &index)
{
    QString path = index.data().toString();
    if(path.isEmpty())
        return;

    path = path.remove("/database/"+Library::instance()->defaultFileName());
    openLibrary(path);
}
