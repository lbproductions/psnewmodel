#include "settings.h"

#include <QSettings>
#include <QStandardPaths>

GameSettings::GameSettings(QObject *_parent):
    QObject(_parent),
    m_sort(SortByPosition),
    m_pointsDisplay(TotalPoints),
    m_tableDisplay(RoundPoints),
    m_showExtraRows(true)
{
    QSettings settings;
    m_sort = static_cast<PlayerSort>(settings.value("gamewindow/settings/playerSort", 1).toInt());
    m_pointsDisplay = static_cast<PointsDisplay>(settings.value("gamewindow/settings/pointsDisplay", 0).toInt());
    m_tableDisplay = static_cast<TableDisplay>(settings.value("gamewindow/settings/tableDisplay", 0).toInt());
    m_showExtraRows = settings.value("gamewindow/settings/showExtraRows", true).toBool();
    m_gamePercentageWarning = settings.value("gamewindow/settings/gamePercentageWarning", false).toBool();
}

GameSettings::~GameSettings()
{
    QSettings settings;
    settings.setValue("gamewindow/settings/playerSort", m_sort);
    settings.setValue("gamewindow/settings/pointsDisplay", m_pointsDisplay);
    settings.setValue("gamewindow/settings/tableDisplay", m_tableDisplay);
    settings.setValue("gamewindow/settings/showExtraRows", m_showExtraRows);
    settings.setValue("gamewindow/settings/gamePercentageWarning", m_gamePercentageWarning);
}

QString GameSettings::openFileLocation()
{
    QSettings settings;
    QString dir = settings.value("openFileLocation").toString();
    if(!dir.isEmpty())
        return dir;

    dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

#ifdef Q_OS_MAC
    // Bug in QFileDialog
    // http://stackoverflow.com/questions/16194475/qfiledialoggetopenfilename-doesnt-set-the-initial-directory-on-mac-os-10-8-mo
    dir.append("/someDummyFile");
#endif

    return dir;
}

void GameSettings::saveOpenFileLocation(const QString &location)
{
    QSettings settings;
    settings.setValue("openFileLocation", location);
}

GameSettings &GameSettings::instance()
{
    static GameSettings inst;
    return inst;
}

GameSettings::PlayerSort GameSettings::playerSort()
{
    return m_sort;
}

void GameSettings::setPlayerSort(GameSettings::PlayerSort sort)
{
    m_sort = sort;

    emit playerSortChanged(m_sort);
}

GameSettings::PointsDisplay GameSettings::pointsDisplay()
{
    return m_pointsDisplay;
}

void GameSettings::setPointsDisplay(GameSettings::PointsDisplay display)
{
    m_pointsDisplay = display;

    emit pointsDisplayChanged(m_pointsDisplay);
}

GameSettings::TableDisplay GameSettings::tableDisplay()
{
    return m_tableDisplay;
}

void GameSettings::setTableDisplay(GameSettings::TableDisplay tableDisplay)
{
    m_tableDisplay = tableDisplay;

    emit tableDisplayChanged(m_tableDisplay);
}

bool GameSettings::showExtraRows()
{
    return m_showExtraRows;
}

void GameSettings::setShowExtraRows(bool showExtraRows)
{
    m_showExtraRows = showExtraRows;

    emit showExtraRowsChanged(m_showExtraRows);
}

bool GameSettings::gamePercentageWarning()
{
    return m_gamePercentageWarning;
}

void GameSettings::setGamePercentageWarning(bool warning)
{
    m_gamePercentageWarning = warning;
}
