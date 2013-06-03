#ifndef OVERVIEWDELEGATE_H
#define OVERVIEWDELEGATE_H

#include <QStyledItemDelegate>

class GameOverviewModel;

class OverviewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit OverviewDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    
    GameOverviewModel *model() const;
    void setGameModel(GameOverviewModel *model);

private:
    GameOverviewModel *m_model;
};

#endif // OVERVIEWDELEGATE_H
