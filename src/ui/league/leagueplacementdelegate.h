#ifndef LEAGUEPLACEMENTDELEGATE_H
#define LEAGUEPLACEMENTDELEGATE_H

#include <QStyledItemDelegate>

class LeagueClassementModel;

class LeagueDelegate : public QStyledItemDelegate
{
        Q_OBJECT
    public:
        enum Model {
            PlacementModel,
            ClassementModel
        };

        explicit LeagueDelegate(QObject *parent = 0);
        ~LeagueDelegate();

        void setClassementModel(LeagueClassementModel* model);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

        QSize sizeHint(const QStyleOptionViewItem &option,
                            const QModelIndex &index) const Q_DECL_OVERRIDE;

        void setModel(Model model);
    signals:
        
    public slots:

    private:
        Model m_model;

        LeagueClassementModel* m_classementModel;
        
};

#endif // LEAGUEPLACEMENTDELEGATE_H
