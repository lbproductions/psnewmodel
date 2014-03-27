#include "updater.h"

#include "sparkleupdater.h"

Updater::~Updater()
{
}

Updater *Updater::instanceForPlatform()
{
#ifdef Q_OS_MAC
    return new SparkleUpdater("https://raw.github.com/lbproductions/psnewmodel/master/src/misc/updater/sparkle-appcast.xml");
#elif defined Q_OS_WIN
    return nullptr;
#endif
}
