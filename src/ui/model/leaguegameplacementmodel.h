#ifndef LEAGUEGAMEPLACEMENTMODEL_H
#define LEAGUEGAMEPLACEMENTMODEL_H

#include <QSharedPointer>
#include <QAbstractTableModel>

class League;

class LeagueGamePlacementModel: public QAbstractTableModel
{
    public:
        LeagueGamePlacementModel(QObject *parent = 0);

        QSharedPointer<League> league() const;
        void setLeague(const QSharedPointer<League> &league);

        Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
        int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
        int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

        void setChanged();

    private:
        QSharedPointer<League> m_league;
};

#endif // LEAGUEGAMEPLACEMENTMODEL_H
