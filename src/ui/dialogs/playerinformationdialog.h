#ifndef PLAYERINFORMATIONDIALOG_H
#define PLAYERINFORMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class PlayerInformationDialog;
}

class Player;
template<class T>
class QpSortFilterProxyObjectModel;

class PlayerInformationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlayerInformationDialog(QWidget *parent = 0);
    ~PlayerInformationDialog();
    
    QSharedPointer<Player> player() const;
    void setPlayerFromModel(QpSortFilterProxyObjectModel<Player> *model, int index);

public slots:
    void accept();

    void nextPlayer();
    void previousPlayer();

    void saveCurrentPlayer();

private slots:
    void on_actionClose_triggered();

private:
    void setPlayer(const QSharedPointer<Player> &player);

    Ui::PlayerInformationDialog *ui;

    int m_currentIndex;
    QpSortFilterProxyObjectModel<Player> *m_model;
    QSharedPointer<Player> m_player;
    QPushButton *m_pushButtonNextPlayer;
    QPushButton *m_pushButtonPrevPlayer;
};

#endif // PLAYERINFORMATIONDIALOG_H
