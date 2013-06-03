#ifndef PLAYERINFORMATIONDIALOG_H
#define PLAYERINFORMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class PlayerInformationDialog;
}

class Player;

class PlayerInformationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlayerInformationDialog(QWidget *parent = 0);
    ~PlayerInformationDialog();
    
    QSharedPointer<Player> player() const;
    void setPlayer(const QSharedPointer<Player> &player);

public slots:
    void accept();

private:
    Ui::PlayerInformationDialog *ui;

    QSharedPointer<Player> m_player;
};

#endif // PLAYERINFORMATIONDIALOG_H
