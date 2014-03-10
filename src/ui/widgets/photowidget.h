#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QWidget>

class Game;
class ClickableImageLabel;
class PhotoPreviewWidget;
class MainWindow;

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

    void setMainWindow(MainWindow* mainWindow);

private slots:
    void onPreviewImageClicked(ClickableImageLabel* label);
    void onPreviewWidgetDoubleClicked();
    void onBackButtonClicked();
    void onPhotoAdded();

private:
    QHash<int, QStringList> checkForPhotos(QSharedPointer<Game> game);
    QStringList photoSuffix();

    Ui::PhotoWidget *ui;

    QHash<QSharedPointer<Game>, QHash<int, QStringList> > m_games;

    ClickableImageLabel* m_selectedLabel;

    QList<PhotoPreviewWidget*> m_previewWidgets;

    MainWindow* m_mainWindow;

    int depth;
};

#endif // PHOTOWIDGET_H
