#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QScrollArea>

class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit ScrollArea(QWidget *parent = 0);

    QList<QWidget *> fixedWidgets() const;
    void setFixedWidgets(const QList<QWidget *> &fixedWidgets);
    void addFixedWidget(QWidget *widget);

    void setWidgetResizable(bool resizable);
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;
    bool event(QEvent *e) Q_DECL_OVERRIDE;

protected:
    void scrollContentsBy(int dx, int dy) Q_DECL_OVERRIDE;
    
    void adjustPositions();

    QList<QWidget *> m_fixedWidgets;
};

#endif // SCROLLAREA_H
