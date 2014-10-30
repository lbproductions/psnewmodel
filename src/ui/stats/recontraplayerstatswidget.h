#ifndef RECONTRAPLAYERSTATSWIDGET_H
#define RECONTRAPLAYERSTATSWIDGET_H

#include <QWidget>

class Player;

class ReContraPlayerStatsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReContraPlayerStatsWidget(QWidget *parent = 0);

    QSharedPointer<Player> player() const;
    void setPlayer(const QSharedPointer<Player> &player);

    int rePercentage() const;
    void setRePercentage(int rePercentage);

    int reWinPercentage() const;
    void setReWinPercentage(int reWinPercentage);

    int contraWinPercentage() const;
    void setContraWinPercentage(int contraWinPercentage);

protected:
    void paintEvent(QPaintEvent *);

    QColor percentageColor(double percentage);
    int valueToCoordinates(int value);

private:
    QSharedPointer<Player> m_player;

    int m_rePercentage;
    int m_reWinPercentage;
    int m_contraWinPercentage;
};

#endif // RECONTRAPLAYERSTATSWIDGET_H
