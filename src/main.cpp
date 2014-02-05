#include <QApplication>

#include <library.h>
#include <ui/startwindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontShowIconsInMenus, true);
    a.setApplicationName("psnewmodel");
    a.setOrganizationName("LB Productions");

    Library library;
    if(!library.open())
        return 0;

    StartWindow startWindow;
    startWindow.show();

    int ret = a.exec();

    library.close();

    return ret;
}
