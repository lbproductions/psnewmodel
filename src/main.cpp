#include <QApplication>

#include <library.h>
#include <misc/updater/updater.h>
#include <ui/chooselibrarywidget.h>
#include <misc/cocoainitializer.h>
#include <misc/crashreporter.h>

#include <QSettings>

void crash() {
    delete reinterpret_cast<QString*>(0xFEE1DEAD);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontShowIconsInMenus, true);
    a.setApplicationName("psnewmodel");
    a.setOrganizationName("LB Productions");
    a.setOrganizationDomain("lbproductions.github.com");
    a.setApplicationVersion(APP_VERSION);

    CocoaInitializer cocoaInitializer;
    Q_UNUSED(cocoaInitializer);

    Updater *updater = Updater::instanceForPlatform();
    updater->checkForUpdatesInBackground();

    CrashReporter::init();

    if(a.arguments().contains("-C")) {
        QSettings s;
        s.clear();
    }

    ChooseLibraryWidget *mainWindow = new ChooseLibraryWidget;
    mainWindow->showOrOpenLibrary();

    int ret = a.exec();

    Library::instance()->close();
    delete updater;

    return ret;
}
