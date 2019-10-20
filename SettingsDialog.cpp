#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, Settings& s, LotPool* l)
    : QDialog(parent)
{
    ui.setupUi(this);
    settings = &s;
    lotPool = l;
    iniUI();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::iniUI() {
    ui.greetingEdit->setText(settings->GetSetting("greeting"));
    ui.historySpinBox->setValue(lotPool->GetHistoryLen());
    int state = settings->GetSetting("lottingstate").toInt();
    switch (state) {
    case 0:
        ui.radioButton_d->setChecked(true);
        break;
    case 1:
        ui.radioButton_ro->setChecked(true);
        break;
    case 2:
        ui.radioButton_ru->setChecked(true);
        break;
    default:
        break;
    }    
}

void SettingsDialog::on_btnOk_clicked() {
    settings->SetSetting("greeting", ui.greetingEdit->text());
    lotPool->SetHistory(ui.historySpinBox->value());
    if (ui.radioButton_d->isChecked())
        settings->SetSetting("lottingstate", "0");
    else if (ui.radioButton_ro->isChecked())
        settings->SetSetting("lottingstate", "1");
    else
        settings->SetSetting("lottingstate", "2");
    accept();
}