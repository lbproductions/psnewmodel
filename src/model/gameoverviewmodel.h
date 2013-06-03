#ifndef GAMEOVERVIEWMODEL_H
#define GAMEOVERVIEWMODEL_H

#include <QAbstractTableModel>

#include <QSharedPointer>

class Game;

class GameOverviewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum ExtraRows {
        HochzeitenRow
    };
    enum ExtraColumns {
        TotalColumn
    };
    enum Roles {
        HochzeitMitspielerColorRole = Qt::UserRole + 1
    };

    explicit GameOverviewModel(QObject *parent = 0);

    QSharedPointer<Game> game() const;
    void setGame(const QSharedPointer<Game> &game);

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<Game> m_game;

    int m_extraColumnCount;
    int m_extraRowsCount;
    
};

#endif // GAMEOVERVIEWMODEL_H
