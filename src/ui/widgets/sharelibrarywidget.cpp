#include "sharelibrarywidget.h"
#include "ui_sharelibrarywidget.h"

#include <library.h>

#include <QPushButton>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

ShareLibraryWidget::ShareLibraryWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShareLibraryWidget)
{
    ui->setupUi(this);

    ui->labelError->setVisible(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Share"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ShareLibraryWidget::~ShareLibraryWidget()
{
    delete ui;
}

void ShareLibraryWidget::on_buttonBox_accepted()
{
    // TODO: Errors in Messagebox

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

    if(!dir.cd("Dropbox")) {
        qWarning() << "Could not cd into Dropbox";
        reject();
        return;
    }

    QString dirName = QApplication::applicationName()
            + QDir::separator()
            + "Shared"
            + QDir::separator()
            + ui->lineEditShareName->text();

    if(!dir.mkpath(dirName) || !dir.cd(dirName)) {
        qWarning() << "Could not create or cd to path:";
        qWarning() << dirName;
        reject();
        return;
    }

    QFile file(Library::fileNameFromSettings());
    Q_ASSERT(file.exists());

    if(!file.copy(dir.absoluteFilePath(Library::defaultFileName()))) {
        qWarning() << "Could not create shared database:";
        qWarning() << dir.absoluteFilePath(file.fileName());
        reject();
        return;
    }

    // TODO: Remove the original file after sharing it?

    Library::restartAndOpenLibrary(file.fileName());
}

void ShareLibraryWidget::on_lineEditShareName_textChanged(const QString &text)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->labelError->setVisible(false);

    if(text.isEmpty())
        return;

    QString dirName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
            + QDir::separator()
            + "Dropbox"
            + QDir::separator()
            + QApplication::applicationName()
            + QDir::separator()
            + "Shared"
            + QDir::separator()
            + text;

    QDir dir(dirName);
    QFile file(dir.absoluteFilePath(Library::defaultFileName()));

    if(file.exists()) {
        ui->labelError->setVisible(true);
        return;
    }

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}
