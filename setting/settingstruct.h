#ifndef SETTINGSTRUCT_H
#define SETTINGSTRUCT_H

#include <QObject>

class SettingStruct : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString downloadPath READ downloadPath WRITE setDownloadPath NOTIFY downloadPathChanged FINAL)
    Q_PROPERTY(bool soundEnabled READ soundEnabled WRITE setSoundEnabled NOTIFY soundEnabledChanged FINAL)
    Q_PROPERTY(bool notificationEnabled READ notificationEnabled WRITE setNotificationEnabled NOTIFY notificationEnabledChanged FINAL)
    Q_PROPERTY(int maxConcurrent READ maxConcurrent WRITE setMaxConcurrent NOTIFY maxConcurrentChanged FINAL)
    Q_PROPERTY(int downloadThrd READ downloadThrd WRITE setDownloadThrd NOTIFY downloadThrdChanged FINAL)

public:
    static SettingStruct* getInstance();
    SettingStruct(const SettingStruct&)=delete;
    SettingStruct operator =(const SettingStruct&)=delete;

public slots:
    QString downloadPath(){return downloadPath_;}
    void setDownloadPath(QString val){downloadPath_=val;emit downloadPathChanged();}
    bool soundEnabled(){return soundEnabled_;}
    void setSoundEnabled(bool val){soundEnabled_=val;emit soundEnabledChanged();}
    bool notificationEnabled(){return notificationEnabled_;}
    void setNotificationEnabled(bool val){notificationEnabled_=val;emit notificationEnabledChanged();}
    int maxConcurrent(){return maxConcurrent_;}
    void setMaxConcurrent(int val){maxConcurrent_=val;emit maxConcurrentChanged();}
    int downloadThrd(){return downloadThrd_;}
    void setDownloadThrd(int val){downloadThrd_=val;emit downloadThrdChanged();}

    void setConfig(QVariantMap &config);

private:
    QString downloadPath_;
    bool soundEnabled_;
    bool notificationEnabled_;
    int maxConcurrent_;
    int downloadThrd_;

private:
    SettingStruct(QObject *parent = nullptr);
    ~SettingStruct();

    void readConfig();

signals:
    void downloadPathChanged();
    void soundEnabledChanged();
    void notificationEnabledChanged();
    void maxConcurrentChanged();
    void downloadThrdChanged();
};

#endif // SETTINGSTRUCT_H
