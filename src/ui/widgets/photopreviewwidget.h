#ifndef PHOTOPREVIEWWIDGET_H
#define PHOTOPREVIEWWIDGET_H

#include <QWidget>

class ClickableImageLabel;
class Game;

namespace Ui {
class PhotoPreviewWidget;
}

class PhotoPreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoPreviewWidget(QWidget *parent = 0);
    ~PhotoPreviewWidget();

    void setPreviewFile(QString file);
    void setGame(QSharedPointer<Game> game);
    QSharedPointer<Game> game();

signals:
    void doubleClicked();
    void imageClicked(ClickableImageLabel*);

private slots:
    void onImageClicked();

private:
    Ui::PhotoPreviewWidget *ui;

    QSharedPointer<Game> m_game;
};

#endif // PHOTOPREVIEWWIDGET_H
