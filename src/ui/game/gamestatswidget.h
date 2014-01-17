#ifndef GAMESTATSWIDGET_H
#define GAMESTATSWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
    class GameStatsWidget;
}

class Game;
class ReContraStatsWidget;

class GameStatsWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit GameStatsWidget(QWidget *parent = 0);
        ~GameStatsWidget();

        void setGame(QSharedPointer<Game> game);

   private slots:
        void onItemClicked(QTreeWidgetItem* item);
        
    private:
        void addWidget(QString name, QWidget* widget);

        Ui::GameStatsWidget *ui;

        QSharedPointer<Game> m_game;

        QHash<QTreeWidgetItem*, int> m_indexes;

        ReContraStatsWidget* m_reContraWidget;

        int m_widgetCounter;
};

#endif // GAMESTATSWIDGET_H
