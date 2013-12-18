#ifndef LEAGUEPLACEMENTDELEGATE_H
#define LEAGUEPLACEMENTDELEGATE_H

#include <QStyledItemDelegate>

class LeagueClassementModel;

class LeaguePlacementDelegate : public QStyledItemDelegate
{
        Q_OBJECT
    public:
        explicit LeaguePlacementDelegate(QObject *parent = 0);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

        QSize sizeHint(const QStyleOptionViewItem &option,
                            const QModelIndex &index) const Q_DECL_OVERRIDE;

        LeagueClassementModel *model() const;
        void setLeagueClassementModel(LeagueClassementModel *model);
    signals:
        
    public slots:

    private:
        LeagueClassementModel* m_model;
        
};

#endif // LEAGUEPLACEMENTDELEGATE_H
