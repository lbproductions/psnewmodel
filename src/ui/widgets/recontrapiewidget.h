#ifndef RECONTRAPIEWIDGET_H
#define RECONTRAPIEWIDGET_H

#include <QWidget>

class Game;

class ReContraPieWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit ReContraPieWidget(QWidget *parent = 0);

        QSize sizeHint() const;

        void setGames(QList<QSharedPointer<Game> > games);
        
    signals:
        
    public slots:

    private:
        void paintEvent(QPaintEvent *event);

        QList<QSharedPointer<Game> > m_games;

        double m_rePerc;
        double m_contraPerc;
        
};

#endif // RECONTRAPIEWIDGET_H
