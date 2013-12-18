#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include <QTreeWidget>

class Game;
class League;

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
        void onLeagueTreeWidgetDoubleClicked(QTreeWidgetItem* item, int column);
        void onNewLeagueButtonPressed();

    private:
        QList<QSharedPointer<Game> > m_games;
        QList<QSharedPointer<League> > m_leagues;

        QTreeWidget* m_treeWidget;
        QTreeWidget* m_leagueTreeWidget;
        
};

#endif // STARTWINDOW_H
