#ifndef ADDDRINKSWIDGET_H
#define ADDDRINKSWIDGET_H

#include <QWidget>

class QHBoxLayout;
class Player;
class Game;
class Drink;
class DrinksListWidget;

class AddDrinksWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit AddDrinksWidget(QWidget *parent = 0);

        void setPlayers(QList<QSharedPointer<Player> > players);

        QSharedPointer<Game> game() const;
        void setGame(const QSharedPointer<Game> &game);

    private slots:
        void addDrinkToWidgets(QSharedPointer<Drink> drink);

    private:
        QWidget *createPlayerWidget(QSharedPointer<Player> player);

        QHBoxLayout *m_layoutPlayerWidgets;
        QList<DrinksListWidget*> m_drinkListWidgets;
        QSharedPointer<Game> m_game;

};

#endif // ADDDRINKSWIDGET_H
