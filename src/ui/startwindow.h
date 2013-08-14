#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include <QTreeWidget>

class Game;

class StartWindow : public QWidget
{
        Q_OBJECT
    public:
        explicit StartWindow(QWidget *parent = 0);
        
    signals:
        
    private slots:
        void onNewGameButtonPressed();
        void onLibraryButtonPressed();
        void onGameTreeWidgetDoubleClicked(QTreeWidgetItem* item, int column);

    private:
        QList<QSharedPointer<Game> > m_games;

        QTreeWidget* m_treeWidget;
        
};

#endif // STARTWINDOW_H
