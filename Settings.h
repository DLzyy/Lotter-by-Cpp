#pragma once

#include <QObject>
#include <QString>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT

public:
    Settings(QObject *parent = Q_NULLPTR);
    ~Settings();
    void Save();
    QString GetSetting(const QString&) const;
    void SetSetting(const QString&, const QString&);

private:
    QHash<QString, QString> configs;
    const QString ORGANIZATION = "Dlzyy";
    const QString APP_NAME = "Draw_Lots";
    void ReadSettings();
};
