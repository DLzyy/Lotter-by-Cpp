#pragma once

#include <QWidget>
#include "ui_LotPoolEdit.h"
#include <QTextBlock> 
#include <qtextdocument.h>
#include <qlabel.h>
#include "LotPool.h"

class LotPoolEdit : public QWidget
{
    Q_OBJECT

public:
    LotPoolEdit(QWidget *parent = Q_NULLPTR);
    ~LotPoolEdit();
    void Save();
    void SetLotPool(LotPool*);
    void SetCountLabel(QLabel* = nullptr);
    void UpdateName();
    LotPool* GetLotPool();

private:
    Ui::LotPoolEdit ui;
    LotPool* lotPool;
    int LotsCount;
    QLabel* countLabel;
    bool contentChanged = false;

private slots:
    void on_textEdit_textChanged();
};
