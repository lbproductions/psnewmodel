#ifndef GAMESTOGETHERWIDGET_H
#define GAMESTOGETHERWIDGET_H

#include <QWidget>
#include <QTreeWidget>

namespace Ui {
    class GamesTogetherWidget;
}

class Game;
class Player;

class GamesTogetherWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit GamesTogetherWidget(QWidget *parent = 0);
        ~GamesTogetherWidget();

        void setData(QList<QSharedPointer<Game> > games, QList<QSharedPointer<Player> > players);

        QTreeWidget* gamesTogetherWidget();

        void update();
        
    private:
        Ui::GamesTogetherWidget *ui;
};

#endif // GAMESTOGETHERWIDGET_H
