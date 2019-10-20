#pragma once
#pragma execution_character_set("utf-8")

#include <QDialog>
#include "ui_LotsDialog.h"
#include "LotPool.h"
#include "LotPoolEdit.h"
#include <qmdisubwindow.h>
#include <QMessageBox>
#include <QInputDialog>
#include <iostream>


class LotsDialog : public QDialog
{
    Q_OBJECT

public:
    LotsDialog(const QString& fileName, QWidget* parent = Q_NULLPTR);
    ~LotsDialog();
    int Display();

    void Save();
    void Undo();

    LotPool* GetActiveLotPool();

private:
    Ui::LotsDialog ui;
    QMap<QString, LotPool> lotPools;
    QString activeLotPoolName;
    bool showing = false;
    
    QString fileName;
    int LoadLotPools();
    int SaveLotPools();
    LotPool& DefaultLotPools(LotPool& l);
    LotPoolEdit* AddLotPool(LotPool& l);

    QList<QString> removedLotPools;
    QList<QString> newLotPools;
    QList<QMdiSubWindow*> newWindows;
    QHash<QString, QString> renameLotPools; // new : old
    QHash<QString, LotPoolEdit*> renameEdits;
    QString AskName(const QString& oldName, bool* ok = nullptr);
    void clearAll();

protected:
    void accept();
    void reject();

private slots:
    void on_mdiArea_subWindowActivated(QMdiSubWindow* arg1);
    void on_btnAdd_clicked();
    void on_btnRemove_clicked();
    void on_btnRename_clicked();
};
