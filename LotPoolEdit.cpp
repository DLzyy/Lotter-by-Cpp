#include "LotPoolEdit.h"

LotPoolEdit::LotPoolEdit(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    countLabel = nullptr;
    lotPool = nullptr;
    LotsCount = 0;
}

LotPoolEdit::~LotPoolEdit()
{
}

void LotPoolEdit::Save() {
    if (!contentChanged)
        return;
    QString str = ui.textEdit->toPlainText();
    std::cerr << "text: " << std::endl << str.toStdString() << std::endl;
    if (lotPool && !str.isNull()) {
        std::cerr << "Set LotPool!" << std::endl;
        ui.textEdit->clear();
        for each (const QString & str in lotPool->SetLotPool(str))
            ui.textEdit->appendPlainText(str);
    }
    contentChanged = false;
}

void LotPoolEdit::SetLotPool(LotPool* l) {
    lotPool = l;
    ui.textEdit->setPlainText("");
    for each (const QString & str in lotPool->GetLotPool()) {
        if (str.isEmpty())
            continue;
        ui.textEdit->appendPlainText(str);
    }
    setWindowTitle(lotPool->GetName());
}

void LotPoolEdit::SetCountLabel(QLabel* l) {
    countLabel = l;
    QString str = "数量:";
    str.append(QString::number(LotsCount));
    countLabel->setText(str);
}

void LotPoolEdit::UpdateName() {
    setWindowTitle(lotPool->GetName());
}

LotPool* LotPoolEdit::GetLotPool() {
    return lotPool;
}

void LotPoolEdit::on_textEdit_textChanged() {
    QTextDocument* doc = ui.textEdit->document();
    int count = 0;
    for (int i = 0; i < doc->blockCount(); i++) {
        QTextBlock line = doc->findBlockByNumber(i);
        QString str = line.text();
        if (!str.isEmpty())
            count += 1;
    }
    if (LotsCount != count) {
        LotsCount = count;
        QString str = "数量:";
        str.append(QString::number(LotsCount));
        if (countLabel)
            countLabel->setText(str);
    }
    contentChanged = true;
}