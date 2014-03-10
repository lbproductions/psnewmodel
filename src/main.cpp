#include <QApplication>

#include <library.h>
#include <application.h>
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
    Application a(argc, argv);
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
    if(a.fileToOpen().isEmpty()) {
        mainWindow->showOrOpenLibrary();
    }
    else {
        mainWindow->openLibrary(a.fileToOpen()+"/database/database.sqlite");
    }

    int ret = a.exec();

    Library::instance()->close();
    delete updater;

    return ret;
}
