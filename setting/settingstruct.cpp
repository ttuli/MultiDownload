#include "settingstruct.h"
#include <QStandardPaths>
#include <QSettings>
#include <QFile>

SettingStruct::SettingStruct(QObject *parent)
    : QObject{parent}
{
    readConfig();
}

SettingStruct::~SettingStruct()
{
    qDebug()<<__FUNCTION__;
    QFile file("setting.ini");
    file.open(QIODevice::WriteOnly);
    file.close();

    qDebug()<<downloadPath_<<maxConcurrent_<<notificationEnabled_<<soundEnabled_<<downloadThrd_;
    QSettings setting("setting.ini",QSettings::IniFormat);
    setting.setValue("downloadPath_",downloadPath_);
    setting.setValue("maxConcurrent_",maxConcurrent_);
    setting.setValue("notificationEnabled_",notificationEnabled_);
    setting.setValue("soundEnabled_",soundEnabled_);
    setting.setValue("downloadThrd_",downloadThrd_);
    setting.sync();
}

void SettingStruct::readConfig()
{
    downloadPath_=QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    maxConcurrent_=2;
    notificationEnabled_=true;
    soundEnabled_=true;
    downloadThrd_=4;

    QSettings setting("setting.ini",QSettings::IniFormat);
    if(!setting.value("downloadPath_").isNull())
        downloadPath_=setting.value("downloadPath_").toString();
    if(!setting.value("maxConcurrent_").isNull())
        maxConcurrent_=setting.value("maxConcurrent_").toInt();
    if(!setting.value("notificationEnabled_").isNull())
        notificationEnabled_=setting.value("notificationEnabled_").toBool();
    if(!setting.value("soundEnabled_").isNull())
        soundEnabled_=setting.value("soundEnabled_").toBool();
    if(!setting.value("downloadThrd_").isNull())
        downloadThrd_=setting.value("downloadThrd_").toInt();

    qDebug()<<downloadPath_<<maxConcurrent_<<notificationEnabled_<<soundEnabled_<<downloadThrd_;
}

SettingStruct *SettingStruct::getInstance()
{
    static SettingStruct obj;
    return &obj;
}

void SettingStruct::setConfig(QVariantMap &config) {}
