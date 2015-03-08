#include "playersdelegate.h"

#include <ui/model/playerslistmodel.h>

#include <QPainter>
#include <QAbstractItemView>

PlayersDelegate::PlayersDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
}

void PlayersDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    QSharedPointer<Player> player = forIndex(index);
    if(!player)
        return;

    painter->save();

    int distanceLeft = 7;
    int itemDistance = 14;
    int rectHeight = option.rect.height()/2;

    painter->save();
    painter->setBrush(player->color());
    QPoint colorPos = option.rect.topLeft() + QPoint(7, rectHeight/2);
    painter->drawRect(QRect(colorPos, QSize(rectHeight, rectHeight)));
    painter->restore();

    QFont font = painter->font();
    font.setPointSize(20);
    font.setBold(true);
    painter->setFont(font);

    QString games = QString::number(player->games().size());
    QString gamesSubtitle = tr("Games");

    int numberHeight = painter->fontMetrics().height();
    int numberWidth = painter->fontMetrics().width(games);

    // Name
    QPoint namesPos = option.rect.topLeft() + QPoint(distanceLeft+rectHeight+distanceLeft, (option.rect.height() - numberHeight)/2);
    drawTitleText(painter, QRect(namesPos, QSize(painter->fontMetrics().width(player->name()), numberHeight)), Qt::AlignCenter, player->name(), option);

    QFont subtitleFont = QFont(font);
    subtitleFont.setPointSize(15);
    subtitleFont.setBold(false);
    painter->setFont(subtitleFont);
    int subtitleHeight = painter->fontMetrics().height();
    int subtitleWidth = painter->fontMetrics().width(gamesSubtitle);

    int gamesOffset = option.rect.height() - numberHeight - subtitleHeight;
    int gamesWidth = qMax(numberWidth, subtitleWidth);

    // Games
    painter->setFont(font);
    QPoint gamesPos = option.rect.topLeft() + QPoint(option.rect.width()-distanceLeft-gamesWidth, gamesOffset/2);
    drawTitleText(painter, QRect(gamesPos, QSize(gamesWidth, numberHeight)), Qt::AlignCenter, games, option);

    painter->setFont(subtitleFont);
    QPoint gamesSubtitlePos = option.rect.topLeft() + QPoint(option.rect.width()-distanceLeft-gamesWidth, gamesOffset/2 + numberHeight);
    drawText(painter, QRect(gamesSubtitlePos, QSize(gamesWidth, subtitleHeight)), Qt::AlignCenter, gamesSubtitle, option);
    painter->restore();

    // Wins
    QString wins = QString::number(player->wins());
    QString winsSubtitle = tr("Wins");

    painter->setFont(font);
    numberWidth = painter->fontMetrics().width(wins);
    painter->setFont(subtitleFont);
    subtitleWidth = painter->fontMetrics().width(winsSubtitle);
    int winsWidth = qMax(numberWidth, subtitleWidth);

    painter->setFont(font);
    QPoint winsPos = option.rect.topLeft() + QPoint(option.rect.width()-distanceLeft-gamesWidth-itemDistance-winsWidth, gamesOffset/2);
    drawTitleText(painter, QRect(winsPos, QSize(winsWidth, numberHeight)), Qt::AlignCenter, wins, option);

    painter->setFont(subtitleFont);
    QPoint winsSubtitlePos = option.rect.topLeft() + QPoint(option.rect.width()-distanceLeft-gamesWidth-itemDistance-winsWidth, gamesOffset/2 + numberHeight);
    drawText(painter, QRect(winsSubtitlePos, QSize(winsWidth, subtitleHeight)), Qt::AlignCenter, winsSubtitle, option);
    painter->restore();

    // Losses
    QString losses = QString::number(player->losses());
    QString lossesSubtitle = tr("Losses");

    painter->setFont(font);
    numberWidth = painter->fontMetrics().width(losses);
    painter->setFont(subtitleFont);
    subtitleWidth = painter->fontMetrics().width(lossesSubtitle);
    int lossesWidth = qMax(numberWidth, subtitleWidth);

    painter->setFont(font);
    QPoint lossesPos = option.rect.topLeft() + QPoint(option.rect.width()-distanceLeft-gamesWidth-2*itemDistance-winsWidth-lossesWidth, gamesOffset/2);
    drawTitleText(painter, QRect(lossesPos, QSize(lossesWidth, numberHeight)), Qt::AlignCenter, losses, option);

    painter->setFont(subtitleFont);
    QPoint lossesSubtitlePos = option.rect.topLeft() + QPoint(option.rect.width()-distanceLeft-gamesWidth-2*itemDistance-winsWidth-lossesWidth, gamesOffset/2 + numberHeight);
    drawText(painter, QRect(lossesSubtitlePos, QSize(lossesWidth, subtitleHeight)), Qt::AlignCenter, lossesSubtitle, option);
    painter->restore();
}

QSize PlayersDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = NoisyGradientItemDelegate::sizeHint(option, index);
    size.setHeight(size.height() + 40);
    return size;
}

QSharedPointer<Player> PlayersDelegate::forIndex(const QModelIndex &index) const
{
    return static_cast<PlayersSortFilterModel *>(view()->model())->objectByIndex(index);
}
