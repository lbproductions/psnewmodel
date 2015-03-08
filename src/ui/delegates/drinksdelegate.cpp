#include "drinksdelegate.h"

#include <ui/model/drinkslistmodel.h>

#include <QPainter>
#include <QAbstractItemView>

DrinksDelegate::DrinksDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
}

void DrinksDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    QSharedPointer<Drink> drink = forIndex(index);
    if(!drink)
        return;

    painter->save();

    int distanceLeft = 7;
    int rectHeight = option.rect.height()/2;

    QFont font = painter->font();
    font.setPointSize(20);
    font.setBold(true);
    painter->setFont(font);
    int numberHeight = painter->fontMetrics().height();

    QPixmap icon = drink->picture().scaled(rectHeight, rectHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPoint iconPos = option.rect.topLeft() + QPoint(7, rectHeight/2 + (rectHeight-icon.height())/2);
    painter->drawPixmap(iconPos, icon);

    // Name
    QPoint namesPos = option.rect.topLeft() + QPoint(distanceLeft+rectHeight+distanceLeft, (option.rect.height() - numberHeight)/2);
    drawTitleText(painter, QRect(namesPos, QSize(painter->fontMetrics().width(drink->name()), numberHeight)), Qt::AlignCenter, drink->name(), option);



    QString drinks = QString::number(drink->liveDrinks().size());
    QString drinksSubtitle = tr("Drinks");

    int numberWidth = painter->fontMetrics().width(drinks);

    QFont subtitleFont = QFont(font);
    subtitleFont.setPointSize(15);
    subtitleFont.setBold(false);
    painter->setFont(subtitleFont);
    int subtitleHeight = painter->fontMetrics().height();
    int subtitleWidth = painter->fontMetrics().width(drinksSubtitle);

    int gamesOffset = option.rect.height() - numberHeight - subtitleHeight;
    int gamesWidth = qMax(numberWidth, subtitleWidth);

    // Drinks
    painter->setFont(font);
    QPoint drinksPos = option.rect.topLeft() + QPoint(option.rect.width()-distanceLeft-gamesWidth, gamesOffset/2);
    drawTitleText(painter, QRect(drinksPos, QSize(gamesWidth, numberHeight)), Qt::AlignCenter, drinks, option);

    painter->setFont(subtitleFont);
    QPoint drinksSubtitlePos = option.rect.topLeft() + QPoint(option.rect.width()-distanceLeft-gamesWidth, gamesOffset/2 + numberHeight);
    drawText(painter, QRect(drinksSubtitlePos, QSize(gamesWidth, subtitleHeight)), Qt::AlignCenter, drinksSubtitle, option);
    painter->restore();

}

QSize DrinksDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = NoisyGradientItemDelegate::sizeHint(option, index);
    size.setHeight(size.height() + 40);
    return size;
}

QSharedPointer<Drink> DrinksDelegate::forIndex(const QModelIndex &index) const
{
    return static_cast<DrinksSortFilterModel *>(view()->model())->objectByIndex(index);
}
