#ifndef NEWROUNDDIALOG_H
#define NEWROUNDDIALOG_H

#include <QDialog>

namespace Ui {
    class NewRoundDialog;
}

class Round;

class NewRoundDialog : public QDialog
{
        Q_OBJECT

    public:

        enum Context{
            NewRound,
            EditRound
        };

        explicit NewRoundDialog(QWidget *parent = 0);
        ~NewRoundDialog();

        QSharedPointer<Round> doppelkopfRound() const;
        void setRound(QSharedPointer<Round> doppelkopfRound, Context context = NewRound);

    private slots:
        void setCurrentPage(int);

        void checkNormalRoundContents();
        void checkHochzeitRoundContents();
        void checkSoloRoundContents();
        void checkTrumpfabgabeRoundContents();

        void save();
        void saveNormalRound();
        void saveHochzeitRound();
        void saveSoloRound();
        void saveTrumpfabgabeRound();

        void on_buttonBox_rejected();

    private:
        Ui::NewRoundDialog *ui;
        QSharedPointer<Round> m_doppelkopfRound;
        Context m_context;
};

#endif // NEWROUNDDIALOG_H
