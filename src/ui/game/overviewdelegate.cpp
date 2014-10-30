#include "overviewdelegate.h"

#include <ui/model/gameoverviewmodel.h>
#include <data/game.h>
#include <data/round.h>

#include <QPainter>
#include <QTextOption>

OverviewDelegate::OverviewDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void OverviewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!m_model || !m_model->game())
        return;

    QSharedPointer<Game> game = m_model->game();
    painter->save();

    int extraRow = -1;
    QPalette palette = static_cast<QWidget *>(this->parent())->palette();

    painter->setFont(model()->data(index, Qt::FontRole).value<QFont>());
    painter->setPen(QPen(Qt::transparent));
    painter->setBrush(palette.color(QPalette::Base));

    QRect r = option.rect;

    painter->drawRect(r);
    painter->setPen(QPen(palette.highlight().color()));
    if(index.column() < game->totalRoundCount())
        painter->drawLine(r.topRight(), r.bottomRight());

    painter->drawLine(r.bottomLeft(), r.bottomRight());
    if(index.row() == 0) {
        painter->drawLine(r.topLeft(), r.topRight());
        r.adjust(0,1,0,0);
    }

    painter->setPen(QPen(Qt::transparent));

    r.adjust(0,0,-1,-1);

    QColor color = index.data(Qt::BackgroundColorRole).value<QColor>();
    if(color.isValid()) {
        painter->setBrush(color);
        painter->drawRect(r);
    }

    // Draw white dividing line
    painter->save();
    extraRow = index.row() - game->players().size();

    painter->setPen(QPen(palette.highlight().color()));
    if(game->players().size() - 1 == index.row()) {
        QRect r2 = r.adjusted(0,0,1,0);
        painter->drawLine(r2.bottomLeft(),r2.bottomRight());

        painter->setPen(QPen(palette.highlight().color().darker(150)));
        r2 = r2.adjusted(-1,0,0,1);
        painter->drawLine(r2.bottomLeft(),r2.bottomRight());
    }
    if(game->players().size() == index.row()) {
        QRect r2 = r.adjusted(0,0,1,0);
        painter->drawLine(r2.topLeft(),r2.topRight());
    }
    if(index.column() == game->totalRoundCount()) {
        QPen pen = painter->pen();
        pen.setWidth(2);
        pen.setBrush(Qt::white);
        painter->setPen(pen);
        QRect r2 = r.adjusted(0,0,1,1);
        painter->drawLine(r2.topLeft(), r2.bottomLeft());
    }
    painter->restore();

    // draw box text
    QString text = index.data().toString();
    if(!text.isEmpty()) {
        painter->setPen(palette.color(QPalette::Text));
        QFont font = painter->font();
        if(index.data(GameOverviewModel::TotalPointsRole).toBool()) {
            font.setBold(true);
        }
        painter->setFont(font);
        QTextOption to;
        to.setAlignment(Qt::AlignCenter);

        painter->drawText(option.rect, text, to);
    }

    // Draw mitspieler
    color = index.data(GameOverviewModel::MitspielerColorRole).value<QColor>();
    if(color.isValid()) {
        painter->setBrush(color);
        painter->drawRect(playerColorRect(option.rect, false));
    }

    // Draw spieler
    color = index.data(Qt::DecorationRole).value<QColor>();
    if(color.isValid()) {
        painter->setBrush(color);
        painter->drawRect(playerColorRect(option.rect, true));
        if(index.data(GameOverviewModel::PflichtSoloRole).toBool()) {
            painter->save();
            QFont font = painter->font();
            font.setPixelSize(15);
            painter->setFont(font);
            painter->setPen(Qt::black);
            painter->drawText(playerColorRect(option.rect, true).translated(1,1),Qt::AlignCenter,"P");
            painter->setPen(Qt::white);
            painter->drawText(playerColorRect(option.rect, true), Qt::AlignCenter, "P");
            painter->restore();
        }
    }

    // Draw schmeissereien
    if(extraRow == GameOverviewModel::SchmeissereienRow) {
        painter->save();
        QList<QColor> colors = index.data(GameOverviewModel::SchmeissereienRole).value<QList<QColor> >();
        if(!colors.isEmpty()) {
            if(colors.size() > 1)
                painter->setPen(QPen(palette.highlight().color()));

            QRect rect = QRect(playerColorRect(option.rect, true));
            double dx = rect.width();
            double dy = 8;
            if(colors.size() > 1)
                dx /= colors.size() - 1;
            dy /= colors.size();
            foreach(QColor color, colors) {
                painter->setBrush(color);
                painter->drawRect(rect);
                rect.adjust(dx,dy,dx,dy);
            }
        }
        painter->restore();
    }

    // draw solo icons
    if(!index.data(GameOverviewModel::SoloTypeRole).isNull()) {
        int type = index.data(GameOverviewModel::SoloTypeRole).toInt();
        painter->setPen(palette.color(QPalette::Text));
        QRect pixmapRect = QRect(option.rect.bottomRight() - QPoint(25,25), option.rect.bottomRight());
        if(type == Round::BubenSolo) {
            static QPixmap pixmap = QPixmap(":/gamewindow/male.png").scaledToHeight(pixmapRect.height(), Qt::SmoothTransformation);
            painter->drawPixmap(pixmapRect, pixmap);
        }
        else if(type == Round::DamenSolo) {
            static QPixmap pixmap = QPixmap(":/gamewindow/female.png").scaledToHeight(pixmapRect.height(), Qt::SmoothTransformation);
            painter->drawPixmap(pixmapRect, pixmap);
        }
        else if(type == Round::StilleHochzeit) {
            static QPixmap pixmap = QPixmap(":/gamewindow/stilleHochzeit.png").scaledToHeight(pixmapRect.height(), Qt::SmoothTransformation);
            painter->drawPixmap(pixmapRect, pixmap);
        }
        else if(type == Round::Fleischlos) {
            static QPixmap pixmap = QPixmap(":/gamewindow/fleischlos1.png").scaledToHeight(pixmapRect.height(), Qt::SmoothTransformation);
            painter->drawPixmap(pixmapRect, pixmap);
        }
        else if(type == Round::TrumpfSolo) {
            static QPixmap pixmap = QPixmap(":/gamewindow/muskeln.png").scaledToHeight(pixmapRect.height(), Qt::SmoothTransformation);
            painter->drawPixmap(pixmapRect, pixmap);
        }
        else if(type == Round::FarbSolo) {
            painter->drawText(QRect(option.rect.bottomRight() - QPoint(15,15), option.rect.bottomRight()), "F");
        }
        else if(type == Round::FalschGespielt) {
            painter->drawText(QRect(option.rect.bottomRight() - QPoint(15,15), option.rect.bottomRight()), "S");
        }
        else if(type == Round::SitzenGelasseneHochzeit) {
            static QPixmap pixmap = QPixmap(":/gamewindow/sitzengelasseneHochzeit.png").scaledToHeight(pixmapRect.height(), Qt::SmoothTransformation);
            painter->drawPixmap(pixmapRect, pixmap);
        }

    }

    /*
    // Draw re-triangle
    if(index.data(GameOverviewModel::IsReRole).toBool()) {
        int triangleSize = 7;
        painter->setBrush(Qt::white);
        painter->setPen(Qt::transparent);
        QPolygonF polygon;
        r.adjust(0,0,1,0);
        polygon << r.topRight() <<
                   QPointF(r.topRight().x()-triangleSize, r.topRight().y()) <<
                   QPointF(r.topRight().x(), r.topRight().y()+triangleSize);
        painter->drawPolygon(polygon);
    }
    */
    painter->restore();
}

GameOverviewModel *OverviewDelegate::model() const
{
    return m_model;
}

void OverviewDelegate::setGameModel(GameOverviewModel *model)
{
    m_model = model;
}

QRect OverviewDelegate::playerColorRect(const QRect &rect, bool first) const
{
    int h = 16;
    int w = qMin(16, rect.width() / 3 + 3);
    int x, y;
    if(first) {
        x = rect.topLeft().x() + rect.width() / 2 - w;
        y = rect.topLeft().y() + 5;
    }
    else {
        x = rect.bottomRight().x() - rect.width() / 2;
        y = rect.bottomRight().y() - h - 5;
    }

    return QRect(QPoint(x, y), QSize(w, h));
}

