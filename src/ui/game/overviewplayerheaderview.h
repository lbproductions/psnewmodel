#ifndef OVERVIEWPLAYERHEADERVIEW_H
#define OVERVIEWPLAYERHEADERVIEW_H

#include <QHeaderView>

class GameInformationModel;
class QTextOption;

class OverviewPlayerHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit OverviewPlayerHeaderView(Qt::Orientation orientation, QWidget *parent = 0);

    GameInformationModel *model() const;
    void setGameModel(GameInformationModel *model);

    bool event(QEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

protected:
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize sectionSizeFromContents(int logicalIndex) const Q_DECL_OVERRIDE;
    void paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const Q_DECL_OVERRIDE;
    
private:
    GameInformationModel *m_model;
    int m_hoverIndex;

    static QRect getSidebarRect(const QRect &rect);
    static QRect getIconRect(int logicalIndex, int playerCount, const QRect &sidebarRect);
    static QTextOption getTextOption();
    static QRect getTextRect(const QRect &rect);

    void paintBackground(const QColor &color, const QRect &rect, QPainter *painter) const;
    void paintSidebar(const QPixmap &icon, int logicalIndex, int playerCount,
                      bool isHoveringActionIndex, QAction *action, const QRect &rect, QPainter *painter) const;
    void paintText(const QString &text, const QRect &rect, QPainter *painter) const;
    void paintBorders(int logicalIndex, int playerCount, const QRect &rect, QPainter *painter) const;
    void paintCardMixerAndPflichtsolo(bool hasPflichtSolo, bool isCurrentCardMixer, const QRect &rect, QPainter *painter) const;
};



#endif // OVERVIEWPLAYERHEADERVIEW_H
