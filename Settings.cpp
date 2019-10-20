#include "Settings.h"


Settings::Settings(QObject *parent)
    : QObject(parent) {
    ReadSettings();
}

Settings::~Settings() {
    Save();
}

void Settings::Save() {
    QSettings settings(ORGANIZATION, APP_NAME);
    QHash<QString, QString>::const_iterator i;
    foreach(const QString & key, configs.keys())
        settings.setValue(key, configs.value(key));
}

QString Settings::GetSetting(const QString& key) const {
    return configs.value(key);
}

void Settings::SetSetting(const QString& key, const QString& value) {
    configs.insert(key, value);
}

void Settings::ReadSettings() {
    QSettings settings(ORGANIZATION, APP_NAME);
    QString s = settings.value("greeting", "Good Luck!").toString();
    configs.insert("greeting", s);
    s = settings.value("lottingstate", "0").toString();
    configs.insert("lottingstate", s);
    s = settings.value("lotsdir", "lots.stm").toString();
    configs.insert("lotsdir", s);
}
