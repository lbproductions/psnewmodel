#ifndef PLACESTATSWIDGET_H
#define PLACESTATSWIDGET_H

#include <QWidget>

namespace Ui {
class PlaceStatsWidget;
}

class PlaceStatsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaceStatsWidget(QWidget *parent = 0);
    ~PlaceStatsWidget();

private:
    Ui::PlaceStatsWidget *ui;
};

#endif // PLACESTATSWIDGET_H
