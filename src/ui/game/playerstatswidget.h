 #ifndef PLAYERSTATSWIDGET_H
#define PLAYERSTATSWIDGET_H

#include <QWidget>
#include <QLabel>

#include <QPushButton>
#include <QProgressBar>

class Player;
class PlayerStatistics;
class Game;

class PlayerStatsWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit PlayerStatsWidget(QSharedPointer<Player> player, QSharedPointer<Game> game, QWidget *parent = 0);

        QString getDrinkString();

    signals:
        
    public slots:
        void update();

    private:
        QColor percentageColor(double percentage);

        QSharedPointer<Player> m_player;
        QSharedPointer<Game> m_game;

        QLabel* m_drinkLabel;
        QProgressBar* m_reContraBar;
        QProgressBar* m_reWinBar;
        QProgressBar* m_contraWinBar;

        PlayerStatistics* m_playerStats;
        
};

#endif // PLAYERSTATSWIDGET_H
