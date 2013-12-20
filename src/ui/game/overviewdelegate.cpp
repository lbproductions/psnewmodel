#include "overviewdelegate.h"

#include <model/gameoverviewmodel.h>
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
    painter->save();

    int extraRow = -1;
    QPalette palette = static_cast<QWidget *>(this->parent())->palette();

    painter->setPen(QPen(Qt::transparent));
    painter->setBrush(palette.color(QPalette::Base));

    QRect r = option.rect;

    painter->drawRect(r);
    painter->setPen(QPen(palette.highlight().color()));
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
    if(m_model) {
        if(m_model->game()) {
            painter->save();
            QSharedPointer<Game> game = m_model->game();
            extraRow = index.row() - game->players().size();

            painter->setPen(QPen(palette.highlight().color()));
            if(game->players().size() - 1 == index.row()) {
                painter->drawLine(r.bottomLeft(),r.bottomRight());

                painter->setPen(QPen(palette.highlight().color().darker(150)));
                QRect r2 = r.adjusted(-1,0,0,1);
                painter->drawLine(r2.bottomLeft(),r2.bottomRight());
            }
            if(game->players().size() == index.row()) {
                QRect r2 = r.adjusted(0,0,1,0);
                painter->drawLine(r2.topLeft(),r2.topRight());
            }
            if(index.column() == game->totalRoundCount()) {
                QRect r2 = r.adjusted(0,0,1,0);
                painter->drawLine(r2.topLeft(), r2.bottomLeft());
            }
            painter->restore();
        }
    }

    // draw box text
    QString text = index.data().toString();
    if(!text.isEmpty()) {
        painter->setPen(palette.color(QPalette::Text));
        QFont font = painter->font();
        font.setPointSize(13);
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
        painter->drawRect(QRect(option.rect.topLeft() + QPoint(16,8), option.rect.topLeft() + QPoint(32,24)));
    }

    // Draw spieler
    color = index.data(Qt::DecorationRole).value<QColor>();
    if(color.isValid()) {
        painter->setBrush(color);
        if(index.data(GameOverviewModel::PflichtSoloRole).toBool()) {
            QPen pen = painter->pen();
            pen.setBrush(Qt::white);
            painter->setPen(pen);
        }
        painter->drawRect(QRect(option.rect.topLeft() + QPoint(4,3), option.rect.topLeft() + QPoint(20,19)));
    }

    // Draw schmeissereien
    if(extraRow == GameOverviewModel::SchmeissereienRow) {
        painter->save();
        QList<QColor> colors = index.data(GameOverviewModel::SchmeissereienRole).value<QList<QColor> >();
        if(!colors.isEmpty()) {
            if(colors.size() > 1)
                painter->setPen(QPen(palette.highlight().color()));

            QRect rect = QRect(option.rect.topLeft() + QPoint(4,3), option.rect.topLeft() + QPoint(20,19));
            double dx = 19;
            double dy = 8;
            dx /= colors.size();
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
            static QPixmap pixmap = QPixmap(":/gamewindow/bubenSolo.png").scaledToHeight(pixmapRect.height());
            painter->drawPixmap(pixmapRect, pixmap);
        }
        else if(type == Round::DamenSolo) {
            static QPixmap pixmap = QPixmap(":/gamewindow/damenSolo.png").scaledToHeight(pixmapRect.height());
            painter->drawPixmap(pixmapRect, pixmap);
        }
        else if(type == Round::StilleHochzeit) {
            static QPixmap pixmap = QPixmap(":/gamewindow/stilleHochzeit.png").scaledToHeight(pixmapRect.height());
            painter->drawPixmap(pixmapRect, pixmap);
        }
        else if(type == Round::Fleischlos) {
            static QPixmap pixmap = QPixmap(":/gamewindow/fleischlos.png").scaledToHeight(pixmapRect.height());
            painter->drawPixmap(pixmapRect, pixmap);
        }
        else if(type == Round::TrumpfSolo) {
            painter->drawText(QRect(option.rect.bottomRight() - QPoint(15,15), option.rect.bottomRight()), "T");
        }
        else if(type == Round::FarbSolo) {
            painter->drawText(QRect(option.rect.bottomRight() - QPoint(15,15), option.rect.bottomRight()), "F");
        }
        else if(type == Round::FalschGespielt) {
            painter->drawText(QRect(option.rect.bottomRight() - QPoint(15,15), option.rect.bottomRight()), "S");
        }
        else if(type == Round::SitzenGelasseneHochzeit) {
            static QPixmap pixmap = QPixmap(":/gamewindow/sitzengelasseneHochzeit.png").scaledToHeight(pixmapRect.height());
            painter->drawPixmap(pixmapRect, pixmap);
        }

    }

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

