#ifndef SCHMEISSEREIDIALOG_H
#define SCHMEISSEREIDIALOG_H

#include <QDialog>

namespace Ui {
    class SchmeissereiWidget;
}

class Round;

class SchmeissereiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SchmeissereiDialog(QWidget *parent = 0);

    ~SchmeissereiDialog();

    QSharedPointer<Round> round() const;
    void setRound(const QSharedPointer<Round> &round);

private slots:
    void save();
    void checkContents();

private:
    Ui::SchmeissereiWidget *ui;

    QSharedPointer<Round> m_round;
};

#endif // SCHMEISSEREIDIALOG_H
