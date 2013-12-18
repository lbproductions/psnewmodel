#ifndef GAMESWIDGET_H
#define GAMESWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
    class GamesWidget;
}

class Game;

class GamesWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit GamesWidget(QWidget *parent = 0);
        ~GamesWidget();

        void setGames(QList<QSharedPointer<Game> > games);
        
    private slots:
        void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

        void on_treeWidgetUnfinshed_itemDoubleClicked(QTreeWidgetItem *item, int column);

    private:
        QTreeWidgetItem* createItem(QTreeWidget* parent, QPixmap statePixmap, QString name, QString players, QString date, QString site, QString complete);

        Ui::GamesWidget *ui;

        QList<QSharedPointer<Game> > m_games;
        QList<QSharedPointer<Game> > m_unfinishedGames;
};

#endif // GAMESWIDGET_H
