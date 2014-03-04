#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QWidget>

class Game;

namespace Ui {
class PhotoWidget;
}

class PhotoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoWidget(QWidget *parent = 0);
    ~PhotoWidget();

    void setGames(QList<QSharedPointer<Game> > games);

private:
    void cleanWidget();

    QList<QString> checkForPhotos(QSharedPointer<Game> game);
    QStringList photoSuffix();

    Ui::PhotoWidget *ui;

    QList<QSharedPointer<Game> > m_games;
};

#endif // PHOTOWIDGET_H
