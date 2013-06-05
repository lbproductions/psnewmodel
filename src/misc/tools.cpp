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
