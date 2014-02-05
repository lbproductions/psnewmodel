#include "updater.h"

#include "sparkleupdater.h"

Updater::~Updater()
{
}

Updater *Updater::instanceForPlatform()
{
    return new SparkleUpdater("https://raw.github.com/lbproductions/psnewmodel/master/src/misc/updater/sparkle-appcast.xml");
}
