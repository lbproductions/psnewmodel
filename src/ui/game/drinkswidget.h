#ifndef DRINKSWIDGET_H
#define DRINKSWIDGET_H

#include <QWidget>

namespace Ui {
class DrinksWidget;
}

class DrinksSortFilterModel;
class DrinkPlayersModel;
class Round;
class QItemSelection;
class LiveDrinkRoundModel;

class DrinksWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrinksWidget(QSharedPointer<Round> round, QWidget *parent = 0);
    ~DrinksWidget();

private slots:
    void on_lineEditSearchDrink_textChanged(const QString &arg1);

    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    void selectDrinkIfNoneSelected();

    void onDrinkSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void addLiveDrinkForSelection();

private:
    Ui::DrinksWidget *ui;
    DrinksSortFilterModel *m_drinksModel;
    DrinkPlayersModel *m_playersModel;
    QSharedPointer<Round> m_round;
    LiveDrinkRoundModel *m_drinkRoundModel;

};

#endif // DRINKSWIDGET_H
