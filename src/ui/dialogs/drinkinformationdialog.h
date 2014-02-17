#ifndef DRINKINFORMATIONDIALOG_H
#define DRINKINFORMATIONDIALOG_H

#include <QDialog>

namespace Ui {
    class DrinkInformationDialog;
}

class Drink;

class DrinkInformationDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit DrinkInformationDialog(QWidget *parent = 0);
        ~DrinkInformationDialog();

        QSharedPointer<Drink> drink() const;
        void setDrink(const QSharedPointer<Drink> &drink);

    public slots:
        void accept() Q_DECL_OVERRIDE;
        void checkData();

    signals:
        void drinkAdded(QSharedPointer<Drink> drink);

    private:
        Ui::DrinkInformationDialog *ui;

        QSharedPointer<Drink> m_drink;
};

#endif // DRINKINFORMATIONDIALOG_H
