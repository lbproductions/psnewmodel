#ifndef BIGGESTDISTANCEWIDGET_H
#define BIGGESTDISTANCEWIDGET_H

#include <QWidget>

namespace Ui {
    class BiggestDistanceWidget;
}

class Game;
class QTreeWidgetItem;

class BiggestDistanceWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit BiggestDistanceWidget(QWidget *parent = 0);
        ~BiggestDistanceWidget();

        void setGames(QList<QSharedPointer<Game> > games);

    private slots:
        void onItemDoubleClicked(QTreeWidgetItem* item);
        
    private:
        Ui::BiggestDistanceWidget *ui;

        QList<QSharedPointer<Game> > m_games;
};

#endif // BIGGESTDISTANCEWIDGET_H
