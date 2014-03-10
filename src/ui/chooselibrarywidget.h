#ifndef CHOOSELIBRARYWIDGET_H
#define CHOOSELIBRARYWIDGET_H

#include <QWidget>

namespace Ui {
class ChooseLibraryWidget;
}

class ChooseLibraryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseLibraryWidget(QWidget *parent = 0);
    ~ChooseLibraryWidget();

    void showOrOpenLibrary();

    void openLibrary(const QString &packagePath);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void on_frameButton1_clicked();
    void on_frameButton1_2_clicked();

    void on_toolButtonOpen_clicked();

    void on_listWidgetRecent_activated(const QModelIndex &index);

private:
    Ui::ChooseLibraryWidget *ui;

    QStringList m_recentLibraries;
    bool m_moving;
    QPoint m_offset;
};

#endif // CHOOSELIBRARYWIDGET_H
