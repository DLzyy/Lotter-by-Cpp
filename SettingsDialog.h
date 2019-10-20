#pragma once

#include <QDialog>
#include "ui_SettingsDialog.h"
#include "Settings.h"
#include "LotPool.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent, Settings& s, LotPool* l);
    ~SettingsDialog();
    void iniUI();

private:
    Ui::SettingsDialog ui;
    Settings* settings;
    LotPool* lotPool;

private slots:
    void on_btnOk_clicked();
};
