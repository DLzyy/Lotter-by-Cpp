#include "LotPool.h"

LotPool::LotPool(QObject *parent)
    : QObject(parent)
{
    current = 0;
    count = 0;
}

LotPool::LotPool(const LotPool& lp) {
    lotPool = lp.lotPool;
    history = lp.history;
    name = lp.name;
    count = lp.count;
    current = lp.current;
}

LotPool::~LotPool()
{
}

LotPool& LotPool::operator=(const LotPool& lp) {
    if (this == &lp)
        return *this;
    lotPool = lp.lotPool;
    history = lp.history;
    name = lp.name;
    count = lp.count;
    current = lp.current;
    return *this;
}

QString LotPool::DrawLot() {
    QString str;
    if (!lotPool.isEmpty()) {
        str = lotPool.at(Random(count));
    }
    else {
        str = NO_LOTS;
    }
    return str;
}

QString LotPool::NextLot(bool order) {
    QString str;
    if (!lotPool.isEmpty()) {
        int n;
        if (order) {
            n = current;
            if (++current >= count)
                current = 0;
        }
        else {
            current = Random(count);
            n = current;
        }
        str = lotPool.at(n);
    }
    else {
        str = NO_LOTS;
    }
    return str;
}

void LotPool::iniHistory() {
    SetHistory(count / 5);
}

void LotPool::SetHistory(int len) {
    history.clear();
    for (int i = 0; i < len && i < count-1; ++i) {
        history.append(-1);
    }
}

void LotPool::SetName(const QString& str) {
    name = str;
}

QDataStream& operator>>(QDataStream& aStream, LotPool& l) {
    qint16 historyLen;
    aStream >> l.name;
    aStream >> l.count;
    aStream >> historyLen;
    l.lotPool.clear();
    QString str;
    for (int i = 0; i < l.count; ++i) {
        aStream >> str;
        l.lotPool << str;
    }
    l.SetHistory(historyLen);
    return aStream;
}

QDataStream& operator<<(QDataStream& aStream, LotPool& l) {
    qint16 historyLen = l.history.size();
    aStream << l.name << l.count << historyLen;
    for each (const QString & str in l.lotPool) {
        aStream << str;
    }
    return aStream;
}

const QList<QString>& LotPool::SetLotPool(const QString& str) {
    if (!str.isEmpty()) {
        QList<QString> list = str.split('\n', QString::SkipEmptyParts);
        lotPool.clear();
        for each (QString s in list) {
            lotPool << s;
            std::cerr << s.toStdString() << std::endl;
        }
        count = lotPool.size();
        iniHistory();
        current = 0;
    }
    else {
        std::cerr << "The lotPool is empty!" << std::endl;
        lotPool.clear();
        count = lotPool.size();
        iniHistory();
        current = 0;
    }
    return lotPool;
}

QString LotPool::GetName() {
    return name;
}

const QList<QString>& LotPool::GetLotPool() const{
    return lotPool;
}

int LotPool::GetHistoryLen() {
    return history.size();
}

int LotPool::Random(int range) {
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int n;
    bool flag = false;
    while (!flag) {
        n = qrand() % range;
        flag = true;
        for each (int i in history) {
            if (n == i) {
                flag = false;
                break;
            }
        }
    }
    if (!history.isEmpty()) {
        history.removeFirst();
        history.append(n);
    }
    return n;
}
