#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWidget(QWidget *parent = 0);

    int maxY() const;
    void setMaxY(int maxY);

    int minY() const;
    void setMinY(int minY);

    int originX() const;
    int originY() const;

    int columnWidth() const;
    void setColumnWidth(int width);

    virtual int translateX(int x) const;
    int translateY(int y) const;

    double pixelsPerPoint() const;

signals:
    void updated();

protected:
    int m_maxY;
    int m_minY;
    int m_columnWidth;
};

#endif // GRAPHWIDGET_H
