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
            DifferenceToLeader
        };

        enum TableDisplay{
            RoundPoints,
            TableTotalPoints,
            DeficitToLeader
        };

        static GameSettings& instance();

        PlayerSort playerSort();
        void setPlayerSort(PlayerSort sort);

        PointsDisplay pointsDisplay();
        void setPointsDisplay(PointsDisplay display);

        TableDisplay tableDisplay();
        void setTableDisplay(TableDisplay tableDisplay);

        bool showExtraRows();
        void setShowExtraRows(bool showExtraRows);

    signals:
        void playerSortChanged(PlayerSort sort);
        void pointsDisplayChanged(PointsDisplay display);
        void tableDisplayChanged(TableDisplay display);
        void showExtraRowsChanged(bool showExtraRows);

    private:
        GameSettings(QObject* _parent = 0);

        static GameSettings* m_instance;

        PlayerSort m_sort;
        PointsDisplay m_pointsDisplay;
        TableDisplay m_tableDisplay;

        bool m_showExtraRows;
};


#endif // SETTINGS_H
