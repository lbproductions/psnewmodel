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

    // Draw white dividing line
    if(m_model) {
        if(m_model->game()) {
            QSharedPointer<Game> game = m_model->game();
            QPen pen = painter->pen();
            pen.setColor(Qt::white);
            painter->setPen(pen);
            if(game->players().size() == index.row()) {
                painter->drawLine(r.topLeft().x(), r.topLeft().y(), r.topRight().x(), r.topRight().y());
            }
            if(index.column() == game->totalRoundCount()) {
                painter->drawLine(r.topLeft(), r.bottomLeft());
            }
            pen.setColor(palette.highlight().color());
            painter->setPen(pen);
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

    color = index.data(GameOverviewModel::MitspielerColorRole).value<QColor>();
    if(color.isValid()) {
        painter->setBrush(color);
        painter->drawRect(QRect(option.rect.topLeft() + QPoint(16,8), option.rect.topLeft() + QPoint(32,24)));
    }

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
        polygon << option.rect.topRight() <<
                   QPointF(option.rect.topRight().x()-triangleSize, option.rect.topRight().y()) <<
                   QPointF(option.rect.topRight().x(), option.rect.topRight().y()+triangleSize);
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

