#pragma once

#include <QObject>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtimer.h>
#include "LotPool.h"

class Lotter : public QObject
{
    Q_OBJECT

public:
    Lotter(QObject *parent, QIcon* i, QString& greeting);
    ~Lotter();
    void SetUI(QPushButton* b, QLabel* l);
    void SetLottingState(int s);
    void SetIcons(QIcon* i);
    void SetGreeting(const QString& greeting);
    void SetLotPool(LotPool* l);
    LotPool* GetLotPool();
    void StopRunning();

private:
    LotPool* lotPool;
    QTimer timer;
    int lottingState;
    QPushButton* mainButton;
    QLabel* outputLabel;
    QIcon* BtnIcons;
    QString greeting;
    bool running;
    void Stop();

private slots:
    void on_mainButton_clicked();
    void nextLot();
};
