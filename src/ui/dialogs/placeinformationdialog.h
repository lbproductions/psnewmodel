#ifndef PLACEINFORMATIONDIALOG_H
#define PLACEINFORMATIONDIALOG_H

#include <QDialog>

class Place;

namespace Ui {
class PlaceInformationDialog;
}

class PlaceInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlaceInformationDialog(QWidget *parent = 0);
    ~PlaceInformationDialog();

    void setPlace(QSharedPointer<Place> place);

public slots:
    void accept() Q_DECL_OVERRIDE;
    void checkData();

signals:
    void placeAdded(QSharedPointer<Place> place);

private:    
    Ui::PlaceInformationDialog *ui;

    QSharedPointer<Place> m_place;
};

#endif // PLACEINFORMATIONDIALOG_H
