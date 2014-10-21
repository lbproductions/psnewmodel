#ifndef PLACESTATSWIDGET_H
#define PLACESTATSWIDGET_H

#include <QWidget>

namespace Ui {
class PlaceStatsWidget;
}

class League;
class Player;

class PlaceStats : public QObject{

public:
    explicit PlaceStats(QObject* parent = 0);

    int wins;
    int losses;
    int games;
    int points;
    double average;
};

class PlaceStatsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaceStatsWidget(QWidget *parent = 0);
    ~PlaceStatsWidget();

    void setLeague(QSharedPointer<League> league);

private slots:
    void on_playerComboBox_currentIndexChanged(int index);

    void clearLayouts();

    void clearLayout(QLayout* layout, bool deleteWidgets = true);

    void update();
private:
    Ui::PlaceStatsWidget *ui;

    QSharedPointer<League> m_league;
};

#endif // PLACESTATSWIDGET_H
