#ifndef ADDPHOTOSDIALOG_H
#define ADDPHOTOSDIALOG_H

#include <QDialog>

namespace Ui {
class AddPhotosDialog;
}

class AddPhotosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPhotosDialog(QWidget *parent = 0);
    ~AddPhotosDialog();

    void setFilesToAdd(QStringList list);

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonNext_clicked();

    void showEvent( QShowEvent * event );

    void on_comboBoxGame_currentIndexChanged(int);

    void on_comboBoxRound_currentIndexChanged(int);

    void on_pushButtonIdentify_clicked();

    void on_pushButtonSkip_clicked();

    QStringList processedFiles();

    void on_comboBoxDefaultPlayer_currentIndexChanged(int index);

signals:
    void processFinishedWithError(QString error);
    void processFinished();

private:
    void loadFile(const QString &fileName);
    void processFile();
    int distance(QDateTime one, QDateTime two);

    Ui::AddPhotosDialog *ui;

    QStringList m_filesToAdd;
    QStringList m_processedFiles;
    QString m_currentFile;
};

#endif // ADDPHOTOSDIALOG_H
