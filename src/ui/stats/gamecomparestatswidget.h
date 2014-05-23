#ifndef GAMECOMPARESTATSWIDGET_H
#define GAMECOMPARESTATSWIDGET_H

#include <QWidget>

class League;
class Game;

namespace Ui {
class GameCompareStatsWidget;
}

class GameCompareStatsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameCompareStatsWidget(QWidget *parent = 0);
    ~GameCompareStatsWidget();

    void setLeague(QSharedPointer<League> league);

    void selectGame(QSharedPointer<Game> game);

private slots:
    void on_radioButtonPlacement_clicked(bool checked);

    void on_radioButtonValue_clicked(bool checked);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void update();

private:
    void showOrOpenLibrary(QShowEvent *);

    Ui::GameCompareStatsWidget *ui;

    QSharedPointer<League> m_league;
    QSharedPointer<Game> m_selectedGame;
};

#endif // GAMECOMPARESTATSWIDGET_H
