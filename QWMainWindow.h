#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include "ui_QWMainWindow.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "LotPool.h"
#include "Lotter.h"
#include "LotsDialog.h"
#include "SimpleWin.h"

class QWMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QWMainWindow(QWidget *parent = Q_NULLPTR);
    ~QWMainWindow();

private:
    Ui::QWMainWindow ui;
    Lotter* lotter;
    Settings settings;
    QMap<QString, LotPool> lotPools;
    LotsDialog* lotsDialog;
    SimpleWin* simpleWin;
    void iniUI();

private slots:
    void on_actSettings_triggered();
    void on_actLotPool_triggered();
    void on_actSimpleWin_triggered();
    void on_simpleWin_hideWin();
};
