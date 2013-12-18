#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
    class SettingsWidget;
}

class SettingsWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit SettingsWidget(QWidget *parent = 0);
        ~SettingsWidget();


    private slots:


        void on_radioButtonPlacement_clicked(bool checked);

        void on_radioButtonPosition_clicked(bool checked);

        void on_radioButtonTotalPoints_clicked(bool checked);

        void on_radioButtonPointsToLeader_clicked(bool checked);

        void on_radioButtonTableDeficit_clicked(bool checked);

        void on_radioButtonTableRoundPoints_clicked(bool checked);

        void on_radioButtonTableTotalPoints_clicked(bool checked);

        void on_checkBoxShowExtraRows_clicked(bool checked);

    private:
        Ui::SettingsWidget *ui;
};


#endif // SETTINGSWIDGET_H
