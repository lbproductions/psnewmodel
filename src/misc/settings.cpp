#include "settings.h"

GameSettings::GameSettings(QObject *_parent):
    QObject(_parent),
    m_sort(SortByPosition),
    m_pointsDisplay(TotalPoints),
    m_tableDisplay(RoundPoints),
    m_showExtraRows(true)
{
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
