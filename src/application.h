#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);

    bool event(QEvent *event);

    bool loadFile(const QString &fileName);

    QString fileToOpen() const;

private:
    QString m_fileToOpen;

};

#endif // APPLICATION_H
