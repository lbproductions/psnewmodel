#ifndef GAMESWIDGET_H
#define GAMESWIDGET_H

#include <QTreeWidget>

namespace Ui {
    class GamesWidget;
}

class Game;

class GamesWidget : public QTreeWidget
{
        Q_OBJECT
        
    public:
        explicit GamesWidget(QWidget *parent = 0);
        ~GamesWidget();

        void setGames(QList<QSharedPointer<Game> > games);
        
    private slots:
        void onItemDoubleClicked(QTreeWidgetItem *item, int column);

    private:
        QTreeWidgetItem* createItem(QPixmap statePixmap, QString name, QString players, QString date, QString site, QString complete);

        QList<QSharedPointer<Game> > m_games;
};

#endif // GAMESWIDGET_H
