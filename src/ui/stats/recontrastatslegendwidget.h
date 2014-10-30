#ifndef RECONTRASTATSLEGENDWIDGET_H
#define RECONTRASTATSLEGENDWIDGET_H

#include <QWidget>

class ReContraStatsLegendWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReContraStatsLegendWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // RECONTRASTATSLEGENDWIDGET_H
