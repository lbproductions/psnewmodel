#ifndef PHOTOROUNDWIDGET_H
#define PHOTOROUNDWIDGET_H

#include <QWidget>

namespace Ui {
class PhotoRoundWidget;
}

class Round;

class PhotoRoundWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoRoundWidget(QWidget *parent = 0);
    ~PhotoRoundWidget();

    void setRound(QSharedPointer<Round> round, QStringList files);

private:
    Ui::PhotoRoundWidget *ui;
};

#endif // PHOTOROUNDWIDGET_H
