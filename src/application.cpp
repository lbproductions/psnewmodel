#include "application.h"

#include <QFileOpenEvent>
#include <QMessageBox>

Application::Application(int &argc, char **argv) :
    QApplication(argc, argv)
{
}

bool Application::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
        return loadFile(static_cast<QFileOpenEvent *>(
                            event)->file());
    default:
        return QApplication::event(event);
    }
}

bool Application::loadFile(const QString &fileName)
{
    m_fileToOpen = fileName;

    QMessageBox box;
    box.setText(fileName);
    box.exec();

    return true;
}

QString Application::fileToOpen() const
{
    return m_fileToOpen;
}
