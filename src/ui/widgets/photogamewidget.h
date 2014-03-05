#ifndef PHOTOGAMEWIDGET_H
#define PHOTOGAMEWIDGET_H

#include <QWidget>

class Game;

namespace Ui {
class PhotoGameWidget;
}

class PhotoGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoGameWidget(QWidget *parent = 0);
    ~PhotoGameWidget();

    void setGame(QSharedPointer<Game> game, QStringList files);

signals:
    void backButtonClicked();

private slots:
     void on_toolButtonBack_clicked();

private:
    Ui::PhotoGameWidget *ui;

    QSharedPointer<Game> m_game;

    QList<QString> m_photoFiles;
};

#endif // PHOTOGAMEWIDGET_H
