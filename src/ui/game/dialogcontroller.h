#ifndef DIALOGCONTROLLER_H
#define DIALOGCONTROLLER_H

#include <QObject>

class GameWindow;

class DialogController : public QObject
{
    Q_OBJECT
public:
    explicit DialogController(GameWindow *parent = 0);

    int dialogHeight() const;
    void setDialogHeight(int dialogHeight);

    int dialogOffsetTop() const;
    void setDialogOffsetTop(int dialogOffsetTop);

    void showDialog(QWidget *widget);
    void closeDialogOnMousePress(const QPoint &mousePos);

    int dialogOffsetLeft() const;
    void setDialogOffsetLeft(int dialogOffsetLeft);

signals:
    void dialogClosed();

public slots:
    void closeDialog();

private:
    GameWindow *m_gameWindow;
    QWidget *m_dialog;
    QWidget *m_widget;

    int m_dialogHeight;
    int m_dialogOffsetTop;
    int m_dialogOffsetLeft;

    void closeDialogWhenDestroyed();

    QWidget *frameWidget(QWidget *widget);
    QRect visibleGeometry(QWidget *widget) const;
    QRect hiddenGeometry(QWidget *widget) const;
};

#endif // DIALOGCONTROLLER_H
