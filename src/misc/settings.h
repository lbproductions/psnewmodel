#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class GameSettings : public QObject
{
        Q_OBJECT
    public:
        enum PlayerSort {
            SortByPlacement,
            SortByPosition
        };

        enum PointsDisplay {
            TotalPoints,
            DifferenceToLeader,
            Drinks
        };

        enum TableDisplay{
            RoundPoints,
            TableTotalPoints,
            DeficitToLeader
        };

        static QString openFileLocation();
        static void saveOpenFileLocation(const QString &location);


        static GameSettings& instance();

        PlayerSort playerSort();
        void setPlayerSort(PlayerSort sort);

        PointsDisplay pointsDisplay();
        void setPointsDisplay(PointsDisplay display);

        TableDisplay tableDisplay();
        void setTableDisplay(TableDisplay tableDisplay);

        bool showExtraRows();
        void setShowExtraRows(bool showExtraRows);

        bool gamePercentageWarning();
        void setGamePercentageWarning(bool warning);

    signals:
        void playerSortChanged(PlayerSort sort);
        void pointsDisplayChanged(PointsDisplay display);
        void tableDisplayChanged(TableDisplay display);
        void showExtraRowsChanged(bool showExtraRows);

    private:
        GameSettings(QObject* _parent = 0);
        ~GameSettings();

        static GameSettings* m_instance;

        PlayerSort m_sort;
        PointsDisplay m_pointsDisplay;
        TableDisplay m_tableDisplay;

        bool m_showExtraRows;

        bool m_gamePercentageWarning;
};


#endif // SETTINGS_H
