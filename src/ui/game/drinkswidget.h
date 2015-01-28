#ifndef DRINKSWIDGET_H
#define DRINKSWIDGET_H

#include <QWidget>

namespace Ui {
class DrinksWidget;
}

class DrinksWidgetSortFilterModel;
class DrinkPlayersModel;
class Round;
class QItemSelection;
class LiveDrinkRoundModel;
class Player;

class DrinksWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrinksWidget(QWidget *parent = 0);
    ~DrinksWidget();

    QSharedPointer<Round> round() const;
    void setRound(const QSharedPointer<Round> &round);

private slots:
    void on_lineEditSearchDrink_textChanged(const QString &arg1);

    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    void selectDrinkIfNoneSelected();

    void onDrinkSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void addLiveDrinkForSelection();
    void removeSelectedDrinks();

    void on_pushButtonAdd_clicked();
    void on_pushButtonRemove_clicked();

    void on_listViewPlayers_doubleClicked(const QModelIndex &index);

private:
    Ui::DrinksWidget *ui;
    DrinksWidgetSortFilterModel *m_drinksModel;
    DrinkPlayersModel *m_playersModel;
    QSharedPointer<Round> m_round;
    LiveDrinkRoundModel *m_drinkRoundModel;

    void showExistingDrinks();
    void showExistingDrinksForPlayer(QSharedPointer<Player> player, QWidget *widget);
};

#endif // DRINKSWIDGET_H
