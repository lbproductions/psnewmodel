#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

class Frame : public QFrame
{
    Q_OBJECT
public:
    explicit Frame(QWidget *parent = 0);

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
};

#endif // FRAME_H
