#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>

class StartWindow : public QWidget
{
        Q_OBJECT
    public:
        explicit StartWindow(QWidget *parent = 0);
        
    signals:
        
    public slots:
        void onNewGameButtonPressed();
        void onLibraryButtonPressed();
        
};

#endif // STARTWINDOW_H
