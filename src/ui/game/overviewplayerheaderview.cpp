#include "overviewplayerheaderview.h"

#include <ui/model/gameinformationmodel.h>
#include <data/game.h>
#include <data/player.h>
#include <misc/tools.h>

#include <QPainter>
#include <QEvent>
#include <QAction>
#include <QHoverEvent>

OverviewPlayerHeaderView::OverviewPlayerHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent),
    m_model(0),
    m_hoverIndex(-1)
{
    setSectionResizeMode(QHeaderView::Fixed);
    setAttribute(Qt::WA_Hover, true);
    setAttribute(Qt::WA_MouseTracking, true);
    setPalette(parentWidget()->palette());
}

QSize OverviewPlayerHeaderView::sizeHint() const
{
    QSize size = QHeaderView::sizeHint();
    size += QSize(45,0);
    size.setHeight(30);
    return size;
}

QSize OverviewPlayerHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    QSize size = QHeaderView::sectionSizeFromContents(logicalIndex);
    size += QSize(65,0);
    size.setHeight(30);
    return size;
}

void OverviewPlayerHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    if(!m_model)
        return;

    painter->save();

    QSharedPointer<Game> game = m_model->game();
    int playerCount = game->players().size();
    int extraRow = logicalIndex - playerCount;

    QSharedPointer<Player> player = model()->headerData(logicalIndex, orientation(), GameInformationModel::PlayerRole).value<QSharedPointer<Player>>();

    bool isCurrentCardMixer = game->state() != Game::Finished
            && player && game->currentCardMixer() == player;
    QColor playerColor;
    if(player)
        playerColor = player->color();

    bool isCurrentlyNotPlaying = player && !game->currentPlayingPlayers().contains(player);

    QAction *action = model()->headerData(extraRow, orientation(), GameInformationModel::ActionRole).value<QAction *>();
    bool isHoveringActionIndex = (action && logicalIndex == m_hoverIndex)
            || (action && action->isChecked());

    // background
    QColor backgroundColor = palette().color(QPalette::Base);
    if(isCurrentCardMixer) {
        backgroundColor = playerColor.darker(250);
    }
    else if(isHoveringActionIndex) {
        backgroundColor = QColor(35,35,35);
    }
    else if(isCurrentlyNotPlaying) {
        backgroundColor = backgroundColor.lighter(170);
    }
    paintBackground(backgroundColor, rect, painter);

    // "sidebar"
    QPixmap icon;
    if(player) {
        icon = Tools::colorPixmap(23,22, playerColor);
    }
    else if(action) {
        icon = action->icon().pixmap(QSize(23,22));
    }
    else {
        icon = model()->headerData(logicalIndex, orientation(), Qt::DecorationRole).value<QPixmap>();
    }

    int position = -1;
    if(playerCount > 5) {
        position = game->currentPlayingPlayers().indexOf(player);
    }
    paintSidebar(icon, logicalIndex, playerCount, position, isHoveringActionIndex, isCurrentCardMixer, action, rect, painter);

    // Text (player name etc)
    QString text;
    if(isHoveringActionIndex)
        text = action->text();
    else
        text = model()->headerData(logicalIndex, orientation()).toString();
    paintText(text, rect, painter, !isCurrentlyNotPlaying);

    // Borders
    paintBorders(logicalIndex, playerCount, rect, painter);

    // Cardmixer and pflichtsoli pixmaps
    bool hasPflichtSolo = game->hasPflichtSolo(player);
    paintCardMixerAndPflichtsolo(hasPflichtSolo, isCurrentCardMixer, rect, painter);

    painter->restore();
}

QRect OverviewPlayerHeaderView::getSidebarRect(const QRect &rect)
{
    QRect sidebarRect = QRect(rect.topLeft(),
                              QSize(rect.height(), rect.height()));
    return sidebarRect.adjusted(0,0, -1,0);
}

QRect OverviewPlayerHeaderView::getIconRect(int logicalIndex, int playerCount, const QRect &sidebarRect)
{
    QRect iconRect = sidebarRect.adjusted(2,3, -3,-5);
    if(logicalIndex == 0)
        iconRect.adjust(0,1,0,0);
    if(logicalIndex == playerCount - 1)
        iconRect.adjust(0,0,0,-1);
    if(logicalIndex < playerCount)
        iconRect.adjust(1,0,-1,1);
    if(logicalIndex >= playerCount)
        iconRect.adjust(0,0,-1,0);
    return iconRect;
}

QTextOption OverviewPlayerHeaderView::getTextOption()
{
    static QTextOption textOption;
    textOption.setWrapMode(QTextOption::NoWrap);
    textOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    return textOption;
}

QRect OverviewPlayerHeaderView::getTextRect(const QRect &rect)
{
    return rect.adjusted(rect.height() + 4,0,0,0);
}

void OverviewPlayerHeaderView::paintBackground(const QColor &color, const QRect &rect, QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::transparent);
    painter->setBrush(color);
    painter->drawRect(rect);
    painter->restore();
}

void OverviewPlayerHeaderView::paintSidebar(const QPixmap &icon,
                                            int logicalIndex,
                                            int playerCount,
                                            int position,
                                            bool isHoveringActionIndex,
                                            bool isCardMixer,
                                            QAction *action,
                                            const QRect &rect, QPainter *painter) const
{
    QRect sidebarRect = getSidebarRect(rect);
    QRect iconRect = getIconRect(logicalIndex, playerCount, sidebarRect);

    painter->save();
    painter->setPen(Qt::transparent);
    painter->setBrush(palette().alternateBase());
    if(isHoveringActionIndex)
        painter->setBrush(QColor(35,35,35));

    if(!isCardMixer)
        painter->drawRect(sidebarRect);

    // player color and icon
    if(!icon.isNull())
        painter->drawPixmap(iconRect, icon);

    if(!isHoveringActionIndex || action->actionGroup()->checkedAction() != action) {
        painter->setPen(palette().highlight().color());
        painter->drawLine(sidebarRect.topRight(),
                          sidebarRect.bottomRight());
    }

    if(position >= 0) {
        painter->setFont(model()->headerData(0, orientation(), Qt::FontRole).value<QFont>());
        painter->setPen(palette().color(QPalette::Text).darker(100));
        painter->drawText(sidebarRect, Qt::AlignCenter, QString::number(position+1));
    }

    painter->restore();
}

void OverviewPlayerHeaderView::paintText(const QString &text, const QRect &rect, QPainter *painter, bool active) const
{
    painter->save();
    QTextOption textOption = getTextOption();
    QRect textRect = getTextRect(rect);
    painter->setFont(model()->headerData(0, orientation(), Qt::FontRole).value<QFont>());
    QColor textColor = palette().color(QPalette::Text);
    if(!active) {
        textColor = textColor.darker(150);
    }
    painter->setPen(textColor);
    painter->drawText(textRect, text, textOption);
    painter->restore();
}

void OverviewPlayerHeaderView::paintBorders(int logicalIndex, int playerCount, const QRect &rect, QPainter *painter) const
{
    bool isFirstExtraRow = logicalIndex == playerCount;
    bool isLastPlayerRow = logicalIndex == playerCount - 1;

    painter->save();
    painter->setPen(palette().highlight().color());
    painter->drawLine(rect.topRight(), rect.bottomRight());
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    if(logicalIndex == 0)
        painter->drawLine(rect.topLeft(), rect.topRight());
    if(isFirstExtraRow) {
        QRect r2 = rect.adjusted(0,0,1,0);
        painter->drawLine(r2.topLeft(),r2.topRight());
    }
    if(isLastPlayerRow) {
        QRect r2 = rect.adjusted(-1,0,0,-1);
        painter->drawLine(r2.bottomLeft(),r2.bottomRight());
        r2.adjust(0,0,0,1);
        painter->setPen(QPen(palette().highlight().color().darker(150)));
        painter->drawLine(r2.bottomLeft(),r2.bottomRight());
    }
    painter->restore();
}

void OverviewPlayerHeaderView::paintCardMixerAndPflichtsolo(bool hasPflichtSolo, bool isCurrentCardMixer, const QRect &rect, QPainter *painter) const
{
    painter->save();

    if(hasPflichtSolo) {
        QRect cRect = rect.adjusted(0,0,-15,0);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::transparent);
        painter->setBrush(Qt::green);
        painter->drawEllipse(QPoint(cRect.topRight().x(), cRect.topRight().y()+cRect.height()/2), 7, 7);
    }

    if(isCurrentCardMixer) {
        QRect cRect = rect.adjusted(0,0,-5,0);
        static QPixmap pixmap = QPixmap(":/sidebar/games.png").scaledToHeight(cRect.height()-10);
        int offset = rect.height() - pixmap.height();
        painter->drawPixmap(rect.topLeft().x() + 5, cRect.topLeft().y()+offset/2, pixmap);
    }

    painter->restore();
}

GameInformationModel *OverviewPlayerHeaderView::model() const
{
    return m_model;
}

void OverviewPlayerHeaderView::setGameModel(GameInformationModel *model)
{
    m_model = model;
}

bool OverviewPlayerHeaderView::event(QEvent *e)
{
    QHoverEvent* hover = static_cast<QHoverEvent*>(e);

    switch (e->type()) {
    case QEvent::HoverLeave:
        m_hoverIndex = -1;
        break;
    case QEvent::HoverMove:
    case QEvent::HoverEnter:
        m_hoverIndex = logicalIndexAt(hover->pos());
        break;
    default:
        break;
    }

    return QHeaderView::event(e);
}


void OverviewPlayerHeaderView::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        m_model->triggerAction(logicalIndexAt(e->pos()) - m_model->game()->players().size());

    QHeaderView::mouseReleaseEvent(e);
}
