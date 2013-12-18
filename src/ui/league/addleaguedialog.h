#ifndef ADDLEAGUEDIALOG_H
#define ADDLEAGUEDIALOG_H

#include <QDialog>

class PlayersListWidget;

class Player;

namespace Ui {
    class AddLeagueDialog;
}

class AddLeagueDialog : public QDialog
{
        Q_OBJECT
        
    public:
        explicit AddLeagueDialog(QWidget *parent = 0);
        ~AddLeagueDialog();

    public slots:
        void checkContent();

        void onPossiblePlayersItemActivated(QSharedPointer<Player> player);
        void onSelectedPlayersItemActivated(QSharedPointer<Player> player);
        
    private slots:
        void on_buttonBox_accepted();

    private:
        Ui::AddLeagueDialog *ui;

        PlayersListWidget* m_possiblePlayers;
        PlayersListWidget* m_selectedPlayers;
};

#endif // ADDLEAGUEDIALOG_H
