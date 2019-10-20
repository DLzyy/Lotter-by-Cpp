#include "QWMainWindow.h"

QWMainWindow::QWMainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);
    iniUI();
}

QWMainWindow::~QWMainWindow() {
    delete lotter;
    delete simpleWin;
    delete lotsDialog;
}

void QWMainWindow::iniUI() {
    QIcon* BtnIcons = new QIcon[2];
    BtnIcons[0].addFile(QString::fromUtf8(":/QWMainWindow/images/\345\274\200\345\247\213.png"), QSize(), QIcon::Normal, QIcon::Off);
    BtnIcons[1].addFile(QString::fromUtf8(":/QWMainWindow/images/\346\232\202\345\201\234.png"), QSize(), QIcon::Normal, QIcon::Off);
    lotter = new Lotter(this, BtnIcons, settings.GetSetting("greeting"));
    lotter->SetUI(ui.mainButton, ui.outputLabel);
    lotter->SetLottingState(settings.GetSetting("lottingstate").toInt());
    lotsDialog = new LotsDialog(settings.GetSetting("lotsdir"), this);
    lotter->SetLotPool(lotsDialog->GetActiveLotPool());
    simpleWin = new SimpleWin(this, lotter);
    connect(simpleWin, SIGNAL(hideWin()), this, SLOT(on_simpleWin_hideWin()));
}

void QWMainWindow::on_actSettings_triggered() {
    lotter->StopRunning();
    SettingsDialog* dialog = new SettingsDialog(this, settings, lotter->GetLotPool());
    dialog->exec();
    lotter->SetGreeting(settings.GetSetting("greeting"));
    lotter->SetLottingState(settings.GetSetting("lottingstate").toInt());
    settings.Save();
    delete dialog;
}

void QWMainWindow::on_actLotPool_triggered() {
    lotter->StopRunning();
    int ret = lotsDialog->Display();
    if (ret == QDialog::Accepted)
        lotter->SetLotPool(lotsDialog->GetActiveLotPool());
}

void QWMainWindow::on_actSimpleWin_triggered() {
    hide();
    simpleWin->Display();
}

void QWMainWindow::on_simpleWin_hideWin() {
    lotter->StopRunning();
    lotter->SetUI(ui.mainButton, ui.outputLabel);
    show();
}

