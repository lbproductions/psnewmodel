#include <QApplication>

#include <library.h>
#include <misc/updater/updater.h>
#include <ui/chooselibrarywidget.h>
#include <misc/cocoainitializer.h>

#include <QSettings>

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

    if(a.arguments().contains("-C")) {
        QSettings s;
        s.clear();
    }

    Library library;
    if(!library.open())
        return 0;

    ChooseLibraryWidget *mainWindow = new ChooseLibraryWidget;
    mainWindow->show();

    int ret = a.exec();

    library.close();
    delete updater;

    return ret;
}
