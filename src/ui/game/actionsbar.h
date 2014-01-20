#ifndef ACTIONSBAR_H
#define ACTIONSBAR_H

#include <QFrame>

namespace Ui {
class ActionsBar;
}

class ActionsBar : public QFrame
{
    Q_OBJECT

public:
    explicit ActionsBar(QWidget *parent = 0);
    ~ActionsBar();

    int topOffest() const;
    void setButtonsOffest(int topOffest);

    void addActionButton(QAction *action);

    void showCardWidget(QWidget *widget);
    void closeCardWidget();

    int cardHeight() const;
    void setCardHeight(int cardHeight);

    int cardOffset() const;
    void setCardOffset(int cardOffset);

private:
    Ui::ActionsBar *ui;

    int m_cardOffset;
    int m_cardHeight;
    int m_buttonsOffset;
    QWidget *m_cardWidget;
};

#endif // ACTIONSBAR_H
