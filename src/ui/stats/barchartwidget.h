#ifndef BARCHARTWIDGET_H
#define BARCHARTWIDGET_H

#include <QWidget>

class Player;

class BarChartWidget : public QWidget
{
    Q_OBJECT
public:
    enum SortingOrder {
        Ascending,
        Descending
    };

    explicit BarChartWidget(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);

    void setValue(QSharedPointer<Player> player, double value);

    void setSortingOrder(SortingOrder order);

    void setMinMax(double min, double max);

    void setMiddle(double middle);

    void setBarDistance(int distance);

    void setBarWidth(int barWidth);

    void setPadding(int left, int right, int top, int bottom);

    void setValueFormatString(QString format);

    void setTitle(QString title);

private:
    QList<QSharedPointer<Player>> sortPlayers();
    void calcMinMax();

    QHash<QSharedPointer<Player>,double> m_values;

    SortingOrder m_sortingOrder;

    double m_min;
    double m_max;
    double m_middle;

    int m_barDistance;
    int m_barWidth;

    int m_paddingLeft;
    int m_paddingRight;
    int m_paddingTop;
    int m_paddingBottom;

    QString m_valueFormatString;
    QString m_title;
};

#endif // BARCHARTWIDGET_H
