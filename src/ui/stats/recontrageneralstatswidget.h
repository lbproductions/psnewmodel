#ifndef RECONTRAGENERALSTATSWIDGET_H
#define RECONTRAGENERALSTATSWIDGET_H

#include <QWidget>

class ReContraGeneralStatsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReContraGeneralStatsWidget(QWidget *parent = 0);

    int reRoundWins() const;
    void setReRoundWins(int reRoundWins);

    int contraRoundWins() const;
    void setContraRoundWins(int contraRoundWins);

    double reGamePointAverage() const;
    void setReGamePointAverage(double reGamePointAverage);

    double contraGamePointAverage() const;
    void setContraGamePointAverage(double contraGamePointAverage);

protected:
    void paintEvent(QPaintEvent *);

private:
    int m_reRoundWins;
    int m_contraRoundWins;

    double m_reGamePointAverage;
    double m_contraGamePointAverage;
};

#endif // RECONTRAGENERALSTATSWIDGET_H
