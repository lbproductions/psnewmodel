#ifndef LEAGUECLASSEMENTMODEL_H
#define LEAGUECLASSEMENTMODEL_H

#include <QAbstractTableModel>
#include <QSharedPointer>

class League;

class LeagueClassementModel: public QAbstractTableModel
{
    public:
        enum Roles {
            ClassementChangeRole = Qt::UserRole + 1
        };

        LeagueClassementModel(QObject *parent = 0);

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

#endif // LEAGUECLASSEMENTMODEL_H
