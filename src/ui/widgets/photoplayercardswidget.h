#ifndef PHOTOPLAYERCARDSWIDGET_H
#define PHOTOPLAYERCARDSWIDGET_H

#include <QWidget>

class Player;

namespace Ui {
class PhotoPlayerCardsWidget;
}

class PhotoPlayerCardsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoPlayerCardsWidget(QWidget *parent = 0);
    ~PhotoPlayerCardsWidget();

    void setPlayer(QSharedPointer<Player> player);

    void setImageFile(QString file);
private:
    Ui::PhotoPlayerCardsWidget *ui;

    QSharedPointer<Player> m_player;
};

#endif // PHOTOPLAYERCARDSWIDGET_H
