#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QWidget>

class Game;
class ClickableImageLabel;

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

private slots:
    void onPreviewImageClicked(ClickableImageLabel* label);
    void onPreviewWidgetDoubleClicked();
    void onBackButtonClicked();

private:
    void cleanWidget(QWidget *widget);

    QHash<int, QStringList> checkForPhotos(QSharedPointer<Game> game);
    QStringList photoSuffix();

    Ui::PhotoWidget *ui;

    QHash<QSharedPointer<Game>, QHash<int, QStringList> > m_games;

    ClickableImageLabel* m_selectedLabel;
};

#endif // PHOTOWIDGET_H
