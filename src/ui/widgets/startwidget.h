#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include "noisygradientitemdelegate.h"

class MainWindow;
class GameSortFilterModel;
class Game;

class UnfinishedGamesDelegate : public NoisyGradientItemDelegate
{
public:
    explicit UnfinishedGamesDelegate(QAbstractItemView *view, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<Game> forIndex(const QModelIndex &index) const;
};

namespace Ui {
class StartWidget;
}

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(MainWindow *parent = 0);
    ~StartWidget();

    void init(MainWindow *window);

private slots:
    void on_listViewGames_doubleClicked(const QModelIndex &index);

    void on_listViewLeague_doubleClicked(const QModelIndex &index);

    void on_lineEditSearch_textChanged(const QString &arg1);

    void on_checkBox_clicked();

private:
    Ui::StartWidget *ui;

    GameSortFilterModel* m_sortModel;
};

#endif // STARTWIDGET_H
