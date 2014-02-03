#include "updater.h"

#include "sparkleupdater.h"

Updater *Updater::instanceForPlatform()
{
    return new SparkleUpdater("https://github.com/lbproductions/psnewmodel/tree/master/src/misc/updater/sparkle-appcast.xml");
}
