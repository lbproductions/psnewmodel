#include "settingswidget.h"
#include "ui_settingswidget.h"

#include "misc/tools.h"
#include "misc/settings.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    ui->radioButtonPlacement->setStyleSheet("color:white");
    ui->radioButtonPosition->setStyleSheet("color:white");
    ui->groupBox->setStyleSheet("color:white");

    if(GameSettings::instance().playerSort() == GameSettings::SortByPlacement) {
        ui->radioButtonPlacement->setChecked(true);
    }
    else if(GameSettings::instance().playerSort() == GameSettings::SortByPosition){
        ui->radioButtonPosition->setChecked(true);
    }

    if(GameSettings::instance().pointsDisplay() == GameSettings::DifferenceToLeader) {
        ui->radioButtonPointsToLeader->setChecked(true);
    }
    else if(GameSettings::instance().pointsDisplay() == GameSettings::TotalPoints) {
        ui->radioButtonTotalPoints->setChecked(true);
    }

    switch(GameSettings::instance().tableDisplay()) {
        case GameSettings::DeficitToLeader:
            ui->radioButtonTableDeficit->setChecked(true);
            break;
        case GameSettings::TableTotalPoints:
            ui->radioButtonTableTotalPoints->setChecked(true);
            break;
        case GameSettings::RoundPoints:
            ui->radioButtonTableRoundPoints->setChecked(true);
            break;
    }

    ui->checkBoxShowExtraRows->setChecked(GameSettings::instance().showExtraRows());
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}
void SettingsWidget::on_radioButtonPlacement_clicked(bool checked)
{
    if(checked) {
        ui->radioButtonPosition->setChecked(false);
        GameSettings::instance().setPlayerSort(GameSettings::SortByPlacement);
    }
}

void SettingsWidget::on_radioButtonPosition_clicked(bool checked)
{
    if(checked) {
        ui->radioButtonPlacement->setChecked(false);
        GameSettings::instance().setPlayerSort(GameSettings::SortByPosition);
    }
}

void SettingsWidget::on_radioButtonTotalPoints_clicked(bool checked)
{
    if(checked) {
        ui->radioButtonTotalPoints->setChecked(false);
        GameSettings::instance().setPointsDisplay(GameSettings::TotalPoints);
    }
}

void SettingsWidget::on_radioButtonPointsToLeader_clicked(bool checked)
{
    if(checked) {
        ui->radioButtonPointsToLeader->setChecked(false);
        GameSettings::instance().setPointsDisplay(GameSettings::DifferenceToLeader);
    }
}

void SettingsWidget::on_radioButtonTableDeficit_clicked(bool checked)
{
    if(checked) {
        ui->radioButtonTableRoundPoints->setChecked(false);
        ui->radioButtonTableTotalPoints->setChecked(false);

        GameSettings::instance().setTableDisplay(GameSettings::DeficitToLeader);
    }
}

void SettingsWidget::on_radioButtonTableRoundPoints_clicked(bool checked)
{
    if(checked) {
        ui->radioButtonTableTotalPoints->setChecked(false);
        ui->radioButtonTableDeficit->setChecked(false);

        GameSettings::instance().setTableDisplay(GameSettings::RoundPoints);
    }
}

void SettingsWidget::on_radioButtonTableTotalPoints_clicked(bool checked)
{
    if(checked) {
        ui->radioButtonTableDeficit->setChecked(false);
        ui->radioButtonTableRoundPoints->setChecked(false);

        GameSettings::instance().setTableDisplay(GameSettings::TableTotalPoints);
    }
}

void SettingsWidget::on_checkBoxShowExtraRows_clicked(bool checked)
{
    GameSettings::instance().setShowExtraRows(checked);
}
