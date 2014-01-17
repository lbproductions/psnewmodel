#include "tools.h"

#include <QFile>
#include <QPainter>
#include <QColor>
#include <QDebug>

#include <data/player.h>

void Tools::setStyleSheetFromResource(const QString &resource, QWidget *widget, const QString &additionalStyles)
{
    QFile file(resource);
    Q_ASSERT_X(file.exists(),
               Q_FUNC_INFO,
               QString("Stylesheet not found: '%1'")
               .arg(resource).toLatin1());
    Q_ASSERT(file.open(QFile::ReadOnly));
    widget->setStyleSheet(QString(file.readAll()).append(additionalStyles));
    file.close();
}

QString Tools::progressBarStyleSheet(QColor color)
{
    QString rgbString = QString("rgb(") + QString::number(qRed(color.rgb())) + ","
            + QString::number(qGreen(color.rgb())) + ","
            + QString::number(qBlue(color.rgb())) + ")";

    QString returnValue = QString("QProgressBar{border: 1px solid white; padding: 1px; border-radius: 7px; background: transparent; width: 15px; color:white;}")
            + " QProgressBar::chunk {background: " + rgbString + "; border-radius: 7px;}";

    return returnValue;
}

QString Tools::darkHeaderViewStyleSheet()
{
    return QString("QHeaderView::section {background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, ")
                                      + "stop:0 #616161, stop: 0.5 #505050,"
                                      + "stop: 0.6 #434343, stop:1 #656565);"
                                      + "color: white;"
                                      + "padding-left: 4px;"
                                      + "border: 1px solid #6c6c6c;}";
}

QVariant Tools::percentageString(double percentage)
{
    if(percentage < 0)
        return QVariant();

    return QString("%1 %").arg(percentage * 100, 4, 'f', 2);
}

QVariant Tools::ifPositive(int number)
{
    if(number > 0)
        return number;

    return QVariant();
}

QVariant Tools::scaledPixmap(const QPixmap &pm, int h, int w)
{
    if(pm.isNull())
        return QVariant();

    return pm.scaled(h,w);
}

QPalette Tools::darkPalette(QWidget *widget)
{
    QPalette darkPalette = widget->palette();
    darkPalette.setColor(QPalette::Window, QColor(71,71,71));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(55,55,55));
    darkPalette.setColor(QPalette::AlternateBase, QColor(49,49,49));
    darkPalette.setColor(QPalette::ToolTipBase, darkPalette.color(QPalette::Base));
    darkPalette.setColor(QPalette::ToolTipText, darkPalette.color(QPalette::WindowText));
    darkPalette.setColor(QPalette::Text, QColor(228,228,228));
    darkPalette.setColor(QPalette::Button, QColor(61,61,61));
    darkPalette.setColor(QPalette::ButtonText, QColor(226,226,226));
    darkPalette.setColor(QPalette::BrightText, QColor(43,43,43));
    darkPalette.setColor(QPalette::Highlight, QColor(108,108,108));
    darkPalette.setColor(QPalette::HighlightedText, QColor(187,187,187));

    return darkPalette;
}

QVariant Tools::playersString(const QList<QSharedPointer<Player> > &ps)
{
    if(ps.isEmpty())
        return QVariant();

    QStringList list;
    foreach(QSharedPointer<Player> p, ps) {
        list.append(p->name());
    }

    return list.join(", ");
}

double Tools::percentage(double value1, double value2)
{
    if(value2 == 0)
        return 0;

    double number = (double) value1 / (double) value2;

    return static_cast<double>(static_cast<int>(number*100+0.5))/100.0;
}

QPixmap Tools::playersColorPixmap(QSharedPointer<Player> playerOne, QSharedPointer<Player> playerTwo, int w, int h)
{
    QPixmap pixmap(w,h);
    QPainter painter(&pixmap);
    painter.setBrush(playerOne->color());
    painter.setPen(Qt::transparent);
    QPolygonF polygon;
    polygon << QPointF(0,0) <<
               QPointF(0,h-1) <<
               QPointF(w-1,0);
    painter.drawPolygon(polygon);
    painter.setBrush(playerTwo->color());
    QPolygonF polygon1;
    polygon1 << QPointF(w-1,h-1) <<
               QPointF(0,h-1) <<
               QPointF(w-1,0);
    painter.drawPolygon(polygon1);
    painter.setBrush(Qt::transparent);
    painter.setPen(QColor(108,108,108));
    painter.drawRect(QRect(0,0,w-1,h-1));
    return pixmap;
}

