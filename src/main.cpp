#include <QApplication>

#include <library.h>
#include <misc/updater/updater.h>
#include <ui/startwindow.h>

#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontShowIconsInMenus, true);
    a.setApplicationName("psnewmodel");
    a.setOrganizationName("LB Productions");
    a.setOrganizationDomain("lbproductions.github.com");

    CocoaInitializer cocoaInitializer;
    Q_UNUSED(cocoaInitializer);

    Updater::instanceForPlatform()->checkForUpdatesInBackground();

    if(a.arguments().contains("-C")) {
        QSettings s;
        s.clear();
    }

    Library library;
    if(!library.open())
        return 0;

    StartWindow startWindow;
    startWindow.show();

    int ret = a.exec();

    library.close();
    delete Updater::instanceForPlatform();
    Qp::database().close();

    return ret;
}
