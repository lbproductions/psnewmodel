#include "tools.h"

#include <QFile>

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

