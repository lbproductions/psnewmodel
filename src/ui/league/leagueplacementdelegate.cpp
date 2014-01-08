#include "leagueplacementdelegate.h"

#include <QPainter>

#include <model/leagueclassementmodel.h>
#include "leaguewindow.h"

LeagueDelegate::LeagueDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void LeagueDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QPalette palette = static_cast<QWidget *>(this->parent())->palette();

    painter->setPen(QPen(palette.color(QPalette::Highlight)));
    painter->setBrush(palette.color(QPalette::Base));

    QRect r = option.rect.adjusted(-1,0,0,-1);
    if(index.row() != 0)
        r.adjust(0,-1,0,0);

    painter->drawRect(r);

    QColor color = index.data(Qt::BackgroundColorRole).value<QColor>();
    if(color.isValid()) {
        painter->setBrush(color);
        painter->drawRect(r);
    }

    // draw box text
    QString text = index.data().toString();
    if(!text.isEmpty()) {
        painter->setPen(palette.color(QPalette::Text));
        QFont font = painter->font();
        font.setPointSize(13);
        font.setBold(true);
        painter->setFont(font);
        QTextOption to;
        to.setAlignment(Qt::AlignCenter);

        painter->drawText(option.rect, text, to);
    }

    QPixmap pixmap = index.data(LeagueClassementModel::ClassementChangeRole).value<QPixmap>();
    if(!pixmap.isNull()) {

        QPixmap scaledPixmap = pixmap.scaledToHeight(option.rect.height()-12);
        int offsetHeight = option.rect.height() - scaledPixmap.height();
        int widthOffset = option.rect.width() - scaledPixmap.width();
        painter->drawPixmap(option.rect.topLeft().x()+widthOffset/2, option.rect.topLeft().y()+offsetHeight/2,
                            scaledPixmap);

        /*
        painter->setBrush(Qt::white);
        painter->drawRect(option.rect.topLeft().x()+widthOffset/2, option.rect.topLeft().y()+offsetHeight/2,
                                           scaledPixmap.width(), scaledPixmap.height());
        */
        //painter->drawPixmap(option.rect, pixmap);
    }

    painter->restore();
}

QSize LeagueDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option,index);
    if(m_model == ClassementModel) {
        if(index.column() < 4) {
            size.setWidth(30);
        }
        else{
            size.setWidth(50);
        }
    }
    if(m_model == PlacementModel) {
        size.setWidth(LeagueWindow::tableWidth);
    }
    return size;
}

void LeagueDelegate::setModel(LeagueDelegate::Model model)
{
    m_model = model;
}
