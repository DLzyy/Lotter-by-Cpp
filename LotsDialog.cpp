#include "LotsDialog.h"

LotsDialog::LotsDialog(const QString& fn, QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    QString str = tr("数量:");
    ui.lotsCountLabel->setText(str);
    fileName = fn;
    if (LoadLotPools()) {
        LotPool l;
        DefaultLotPools(l);
        AddLotPool(l);
        activeLotPoolName = l.GetName();
        Save();
    }
    ui.mdiArea->setViewMode(QMdiArea::TabbedView);
    ui.mdiArea->setTabsMovable(true);
    ui.mdiArea->setTabsClosable(false);
}

LotsDialog::~LotsDialog()
{
}

void LotsDialog::Save() {
    for each (const QString & name in removedLotPools)
        lotPools.remove(name);
    clearAll();

    for each (QMdiSubWindow* w in ui.mdiArea->subWindowList()) {
        LotPoolEdit* e = static_cast<LotPoolEdit*>(w->widget());
        e->Save();
    }
    SaveLotPools();
}

void LotsDialog::Undo() {
    std::cerr << "Undo" << std::endl;
    for each (const QString & newName in renameLotPools.keys()) {
        QString oldName = renameLotPools.value(newName);
        LotPool& lpNew = lotPools.take(newName);
        lpNew.SetName(oldName);
        lotPools.insert(oldName, lpNew);
        renameEdits[newName]->SetLotPool(&lotPools[oldName]);
    }
    for each (QMdiSubWindow * w in newWindows) {
        ui.mdiArea->setActiveSubWindow(w);
        ui.mdiArea->closeActiveSubWindow();
    }
    for each (const QString & name in newLotPools)
        lotPools.remove(name);
    for each (const QString & name in removedLotPools)
        AddLotPool(lotPools[name]);
    clearAll();
}

int LotsDialog::Display() {
    showing = true;
    return exec();
}

LotPool* LotsDialog::GetActiveLotPool() {
    return &lotPools[activeLotPoolName];
}

int LotsDialog::LoadLotPools() {
    QFile aFile(fileName);
    if (!aFile.exists())
        return 1;
    if (!(aFile.open(QIODevice::ReadOnly)))
        return 1;
    QDataStream aStream(&aFile);
    aStream.setVersion(QDataStream::Qt_5_13);
    qint16 count;
    aStream >> count;
    aStream >> activeLotPoolName;
    for (int i = 0; i < count; ++i) {
        LotPool l;
        aStream >> l;
        AddLotPool(l);
    }
    aFile.close();
    return 0;
}

int LotsDialog::SaveLotPools() {
    QFile aFile(fileName);
    if (!(aFile.open(QIODevice::WriteOnly | QIODevice::Truncate)))
        return 1;
    QDataStream aStream(&aFile);
    aStream.setVersion(QDataStream::Qt_5_13);
    qint16 count = lotPools.size();
    aStream << count;
    aStream << activeLotPoolName;
    QMap <QString, LotPool>::iterator i;
    for (i = lotPools.begin(); i != lotPools.end(); ++i) {
        LotPool l = i.value();
        aStream << l;
    }
    aFile.close();
    return 0;
}

LotPool& LotsDialog::DefaultLotPools(LotPool& l) {
    static int count = 0;
    QString str = tr("数量:0");
    ui.lotsCountLabel->setText(str);
    QString name = "New lot pool";
    if (count++) {
        name.append("(" + QString::number(count) + ")");
    }
    l.SetName(name);
    l.SetLotPool("");
    return l;
}

LotPoolEdit* LotsDialog::AddLotPool(LotPool& l) {
    lotPools.insert(l.GetName(), l);
    LotPoolEdit* lotPoolEdit = new LotPoolEdit(this);
    lotPoolEdit->SetLotPool(&lotPools[l.GetName()]);
    lotPoolEdit->setAttribute(Qt::WA_DeleteOnClose);
    ui.mdiArea->addSubWindow(lotPoolEdit);
    lotPoolEdit->SetCountLabel(ui.lotsCountLabel);
    lotPoolEdit->show();
    return lotPoolEdit;
}

QString LotsDialog::AskName(const QString& oldName, bool* ok) {
    QString n;
    showing = false;
    do {
        n = QInputDialog::getText(this, tr("获取名称"), tr("请输入名称"), QLineEdit::Normal, oldName, ok);
        if (*ok) {
            if (!n.isEmpty()) {
                bool repeat = false;
                for each (const QString & str in lotPools.keys()) {
                    if (n == str) {
                        repeat = true;
                        break;
                    }
                }
                if (!repeat)
                    break;
                else
                    QMessageBox::warning(this, tr("警告"), tr("名称已存在！"), QMessageBox::Ok);
            }
            else
                QMessageBox::warning(this, tr("警告"), tr("名称不能为空！"), QMessageBox::Ok);
        }
        else {
            n = oldName;
            break;
        }
    } while (1);
    showing = true;
    return n;
}

void LotsDialog::clearAll() {
    removedLotPools.clear();
    newLotPools.clear();
    renameLotPools.clear();
    renameEdits.clear();
    newWindows.clear();
}

void LotsDialog::accept() {
    showing = false;
    Save();
    QDialog::accept();
}

void LotsDialog::reject() {
    showing = false;
    Undo();
    QDialog::reject();
}

void LotsDialog::on_btnAdd_clicked() {
    bool ok = false;
    LotPool l;
    DefaultLotPools(l);
    l.SetName(AskName(l.GetName(), &ok));
    if (ok) {
        LotPoolEdit* le = AddLotPool(l);
        newWindows.append(ui.mdiArea->activeSubWindow());
        newLotPools.append(l.GetName());
        activeLotPoolName = l.GetName();
    }
}

void LotsDialog::on_btnRemove_clicked() {
    QMdiSubWindow* w = ui.mdiArea->activeSubWindow();
    LotPoolEdit* e = static_cast<LotPoolEdit*>(w->widget());
    QString deletedName = e->GetLotPool()->GetName();
    removedLotPools.append(deletedName);
    ui.mdiArea->closeActiveSubWindow();
    ui.mdiArea->setViewMode(QMdiArea::TabbedView);
}

void LotsDialog::on_btnRename_clicked() {
    LotPoolEdit* e = static_cast<LotPoolEdit*>(ui.mdiArea->activeSubWindow()->widget());
    LotPool* l = e->GetLotPool();
    bool ok = false;
    QString oldName = l->GetName();
    QString n = AskName(oldName, &ok);
    if (ok && n != oldName) {
        renameLotPools.insert(n, oldName);
        renameEdits.insert(n, e);
        l->SetName(n);
        LotPool lpOld = lotPools.take(oldName);
        lotPools.insert(n, lpOld);
        e->SetLotPool(&lotPools[n]);
        activeLotPoolName = n;
    }
}

void LotsDialog::on_mdiArea_subWindowActivated(QMdiSubWindow* arg1) {
    if (ui.mdiArea->subWindowList().count() == 0) {
        LotPool l;
        DefaultLotPools(l);
        AddLotPool(l);
    }
    else if (showing){
        LotPoolEdit* e = static_cast<LotPoolEdit*>(ui.mdiArea->activeSubWindow()->widget());
        activeLotPoolName = e->GetLotPool()->GetName();
    }
}