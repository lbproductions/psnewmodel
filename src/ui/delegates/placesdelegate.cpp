#include "placesdelegate.h"

#include <ui/model/placeslistmodel.h>

#include <QPainter>
#include <QAbstractItemView>

PlacesDelegate::PlacesDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
}

void PlacesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    QSharedPointer<Place> place = forIndex(index);
    if(!place)
        return;

    painter->save();

    int distanceLeft = 7;
    int rectHeight = option.rect.height()/2;

    QFont font = painter->font();
    font.setPointSize(20);
    font.setBold(true);
    painter->setFont(font);
    int numberHeight = painter->fontMetrics().height();

    QPoint iconPos = option.rect.topLeft() + QPoint(7, rectHeight/2);
    painter->drawPixmap(iconPos, place->cityEmblem().scaled(rectHeight, rectHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Name
    QPoint namesPos = option.rect.topLeft() + QPoint(distanceLeft+rectHeight+distanceLeft, (option.rect.height() - numberHeight)/2);
    drawTitleText(painter, QRect(namesPos, QSize(painter->fontMetrics().width(place->displayString()), numberHeight)), Qt::AlignCenter, place->displayString(), option);



    QString games = QString::number(place->games().size());
    QString gamesSubtitle = tr("Games");

    int numberWidth = painter->fontMetrics().width(games);

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

}

QSize PlacesDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = NoisyGradientItemDelegate::sizeHint(option, index);
    size.setHeight(size.height() + 40);
    return size;
}

QSharedPointer<Place> PlacesDelegate::forIndex(const QModelIndex &index) const
{
    return static_cast<PlacesSortFilterModel *>(view()->model())->objectByIndex(index);
}
