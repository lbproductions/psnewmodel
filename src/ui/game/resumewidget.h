#ifndef RESUMEWIDGET_H
#define RESUMEWIDGET_H

#include <QWidget>

namespace Ui {
class ResumeWidget;
}

class ResumeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResumeWidget(QWidget *parent = 0);
    ~ResumeWidget();

signals:
    void widgetClicked();

private:
    void mousePressEvent(QMouseEvent *);

    Ui::ResumeWidget *ui;
};

#endif // RESUMEWIDGET_H
