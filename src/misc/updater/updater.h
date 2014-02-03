#ifndef UPDATER_H
#define UPDATER_H

class Updater
{
public:
    virtual void checkForUpdates() = 0;
    virtual void checkForUpdatesInBackground() = 0;

    static Updater *instanceForPlatform();
};

#endif // UPDATER_H
