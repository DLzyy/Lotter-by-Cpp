#include "Lotter.h"

Lotter::Lotter(QObject *parent, QIcon* i, QString& g)
    : QObject(parent)
{
    running = false;
    mainButton = nullptr;
    outputLabel = nullptr;
    lottingState = 0;
    greeting = g;
    SetIcons(i);
    connect(&timer, SIGNAL(timeout()), this, SLOT(nextLot()));
}

Lotter::~Lotter()
{
    delete[] BtnIcons;
}

void Lotter::SetUI(QPushButton* b, QLabel* l) {
    if (mainButton)
        disconnect(mainButton, 0, this, 0);
    if (outputLabel) {
        disconnect(outputLabel, 0, this, 0);
        l->setText(outputLabel->text());
    }
    else
        l->setText(greeting);
    mainButton = b;
    outputLabel = l;
    mainButton->setIcon(BtnIcons[0]);
    connect(mainButton, SIGNAL(clicked()), this, SLOT(on_mainButton_clicked()));
}

void Lotter::SetLottingState(int s) {
    lottingState = s;
}

void Lotter::SetIcons(QIcon* i) {
    BtnIcons = i;
}

void Lotter::SetGreeting(const QString& g) {
    greeting = g;
    outputLabel->setText(greeting);
}

void Lotter::SetLotPool(LotPool* l) {
    StopRunning();
    lotPool = l;
    const QList<QString> & lots = l->GetLotPool();
    for each (const QString & str in lots) {
        std::cerr << str.toStdString() << std::endl;
    }
}

LotPool* Lotter::GetLotPool() {
    return lotPool;
}

void Lotter::Stop() {
    timer.stop();
    mainButton->setIcon(BtnIcons[0]);
    running = false;
}

void Lotter::StopRunning() {
    if (running)
        Stop();
}

void Lotter::on_mainButton_clicked() {
    switch (lottingState) {
    case 0:
        outputLabel->setText(lotPool->DrawLot());
        break;
    case 1:
    case 2:
        if (!running) {
            mainButton->setIcon(BtnIcons[1]);
            nextLot();
            running = true;
        }
        else {
            Stop();
        }
        break;
    default:
        break;
    }
}

void Lotter::nextLot() {
    QString str;
    if (lottingState == 1)
        str = lotPool->NextLot(true);
    else
        str = lotPool->NextLot(false);
    outputLabel->setText(str);
    if (str != lotPool->NO_LOTS)
        timer.start(20);
}