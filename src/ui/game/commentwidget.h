#ifndef COMMENTWIDGET_H
#define COMMENTWIDGET_H

#include <QWidget>

namespace Ui {
    class CommentWidget;
}

class Game;

class CommentWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit CommentWidget(QWidget *parent = 0);
        ~CommentWidget();

        void setGame(QSharedPointer<Game> game);
        
    private slots:
        void on_textEditComment_textChanged();

        void on_lineEditName_editingFinished();

    private:
        Ui::CommentWidget *ui;

        QSharedPointer<Game> m_game;
};

#endif // COMMENTWIDGET_H
