#ifndef NEWROUNDDIALOG_H
#define NEWROUNDDIALOG_H

#include <QWidget>

#include <data/round.h>

namespace Ui {
class NewRoundDialog;
}

class NewRoundDialog : public QWidget
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

    void setCurrentPage(Round::Type type);

private slots:
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

    void on_spinBoxNormalRePoints_valueChanged(int);

    void on_spinBoxNormalContraPoints_valueChanged(int);

    void on_spinBoxHochzeitRePoints_valueChanged(int );

    void on_spinBoxHochzeitContraPoints_valueChanged(int );

    void on_spinBoxSoloRePoints_valueChanged(int );

    void on_spinBoxSoloContraPoints_valueChanged(int );

    void on_spinBoxTrumpfabgabeRePoints_valueChanged(int );

    void on_spinBoxTrumpfabgabeContraPoints_valueChanged(int );

private:
    Ui::NewRoundDialog *ui;
    QSharedPointer<Round> m_doppelkopfRound;
    Context m_context;

    Round::HochzeitDecision hochzeitDecisionFromText(const QString &text);
    QString textFromHochzeitDecision(Round::HochzeitDecision d);
};

#endif // NEWROUNDDIALOG_H
