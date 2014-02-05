#ifndef SHARELIBRARYWIDGET_H
#define SHARELIBRARYWIDGET_H

#include <QDialog>

namespace Ui {
class ShareLibraryWidget;
}

class ShareLibraryWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ShareLibraryWidget(QWidget *parent = 0);
    ~ShareLibraryWidget();

private slots:
    void on_buttonBox_accepted();

    void on_lineEditShareName_textChanged(const QString &arg1);

private:
    Ui::ShareLibraryWidget *ui;
};

#endif // SHARELIBRARYWIDGET_H
