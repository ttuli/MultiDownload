#include "settingdialog.h"
#include <QHBoxLayout>
#include <QQmlContext>
#include <QSettings>
#include <QFile>
#include <QQuickItem>

QString SettingDialog::configName="setting.ini";

SettingDialog::SettingDialog(QWidget *parent)
    :QWidget(parent)
{
    interface_=new QQuickWidget(this);
    interface_->rootContext()->setContextProperty("rootWidget",this);
    interface_->setResizeMode(QQuickWidget::SizeRootObjectToView);
    interface_->setSource(QUrl("qrc:/setting/SettingDialog.qml"));


    QHBoxLayout *qhb=new QHBoxLayout(this);
    qhb->setSpacing(0);
    qhb->setContentsMargins(0,0,0,0);
    qhb->addWidget(interface_);

    setMinimumSize(480,580);
    setMaximumSize(480,580);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlag(Qt::Window);

    readConfig();
}

void SettingDialog::onConfirm()
{
    close();
}

void SettingDialog::onReject()
{
    close();
}

//------目前8个设置项-----------
//downloadPath 下载路径  soundEnabled 是否声音提醒
//notificationEnabled 是否桌面通知  maxConcurrent 最大并发任务
//speedLimit 速度限制  autoStart 自动开始下载
//detectDuplicate 检测重复文件  deleteTempFiles 删除下载过程产生的临时文件
//-----------------------------
void SettingDialog::setConfig(QVariantMap &config)
{
    QMetaObject::invokeMethod(interface_->rootObject(),"setDownloadSettings",Q_ARG(QVariantMap,config));
}

void SettingDialog::readConfig()
{
    QFile file(configName);
    if(!file.exists()){
        file.open(QIODevice::WriteOnly);
        file.close();
        return;
    }

    QSettings setting(configName);
    QVariantMap config;
    config["downloadPath"]=setting.value("downloadPath");
    config["soundEnabled"]=setting.value("soundEnabled");
    config["notificationEnabled"]=setting.value("notificationEnabled");
    config["maxConcurrent"]=setting.value("maxConcurrent");
    config["speedLimit"]=setting.value("speedLimit");
    config["autoStart"]=setting.value("autoStart");
    config["detectDuplicate"]=setting.value("detectDuplicate");
    config["deleteTempFiles"]=setting.value("deleteTempFiles");

    setConfig(config);
}
