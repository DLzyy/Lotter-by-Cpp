#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>
#include <iostream>
#include <qfile.h>
#include <qdatastream.h>
#include <QTime> 

class LotPool : public QObject
{
    Q_OBJECT

public:
    LotPool(QObject *parent= Q_NULLPTR);
    LotPool(const LotPool& lotPool);
    ~LotPool();
    LotPool& LotPool::operator =(const LotPool&);

    QString DrawLot();
    QString NextLot(bool order=true);

    friend QDataStream& operator>>(QDataStream& aStream, LotPool& lotPool);
    friend QDataStream& operator<<(QDataStream& aStream, LotPool& lotPool);

    const QList<QString>& SetLotPool(const QString&);
    void SetHistory(int);
    void SetName(const QString& str);

    QString GetName();
    const QList<QString>& GetLotPool() const;
    int GetHistoryLen();

    const QString NO_LOTS = tr("Ç©³Ø²»ÄÜÎª¿Õ!");

private:
    QList<QString> lotPool;
    QList<int> history;
    QString name;
    int count;
    int current;
    int Random(int);
    void iniHistory();
};
