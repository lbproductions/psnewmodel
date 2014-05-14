#ifndef GAMEINFORMATIONMODEL_H
#define GAMEINFORMATIONMODEL_H

#include <QAbstractTableModel>

#include <QSharedPointer>

class Game;
class QAction;

class GameInformationModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Roles {
        ActionRole = Qt::UserRole + 1,
        PlayerRole
    };

    explicit GameInformationModel(QObject *parent = 0);

    QSharedPointer<Game> game() const;
    void setGame(const QSharedPointer<Game> &game);

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    void triggerAction(int section);
    void setHeaderAction(int section, QAction *action);
    void removeHeaderAction(int section, QAction *action);

    int fontSize() const;
    void setFontSize(int fontSize);

    void updateViews();

private:
    QSharedPointer<Game> m_game;
    int m_fontSize;

    QVariant actionVariant(int section) const;

    QHash<int, QAction *> m_actions;
};

#endif // GAMEINFORMATIONMODEL_H
