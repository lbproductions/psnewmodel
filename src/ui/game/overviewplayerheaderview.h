#ifndef OVERVIEWPLAYERHEADERVIEW_H
#define OVERVIEWPLAYERHEADERVIEW_H

#include <QHeaderView>

class GameOverviewModel;

class OverviewPlayerHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit OverviewPlayerHeaderView(Qt::Orientation orientation, QWidget *parent = 0);

    GameOverviewModel *model() const;
    void setGameModel(GameOverviewModel *model);

protected:
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize sectionSizeFromContents(int logicalIndex) const Q_DECL_OVERRIDE;
    void paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const Q_DECL_OVERRIDE;
    
private:
    GameOverviewModel *m_model;
};

#endif // OVERVIEWPLAYERHEADERVIEW_H
