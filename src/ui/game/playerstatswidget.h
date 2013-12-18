#ifndef PLAYERSTATSWIDGET_H
#define PLAYERSTATSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class Player;
class Game;

class PlayerStatsWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit PlayerStatsWidget(QSharedPointer<Player> player, QSharedPointer<Game> game, QWidget *parent = 0);

        QString getDrinkString();

        QString getRoundsTogetherString();

    signals:
        
    public slots:
        void update();

    private:
        QSharedPointer<Player> m_player;
        QSharedPointer<Game> m_game;

        QLabel* m_drinkLabel;
        QLabel* m_roundsTogetherLabel;
        
};

#endif // PLAYERSTATSWIDGET_H
