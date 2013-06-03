#include "tools.h"

#include <QFile>

void Tools::setStyleSheetFromResource(const QString &resource, QWidget *widget)
{
    QFile file(resource);
    Q_ASSERT_X(file.exists(),
               Q_FUNC_INFO,
               QString("Stylesheet not found: '%1'")
               .arg(resource).toLatin1());
    Q_ASSERT(file.open(QFile::ReadOnly));
    widget->setStyleSheet(QLatin1String(file.readAll()));
    file.close();
}
