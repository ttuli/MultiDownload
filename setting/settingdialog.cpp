#include "settingdialog.h"
#include "settingstruct.h"
#include "messageBox/custommessagebox.h"
#include <QHBoxLayout>
#include <QQmlContext>
#include <QSettings>
#include <QFile>
#include <QQuickItem>
#include <QStandardPaths>

SettingDialog::SettingDialog(QWidget *parent)
    :QWidget(parent)
{
    interface_=new QQuickWidget(this);
    interface_->rootContext()->setContextProperty("rootWidget",this);
    interface_->rootContext()->setContextProperty("config",SettingStruct::getInstance());
    interface_->setResizeMode(QQuickWidget::SizeRootObjectToView);
    interface_->setSource(QUrl("qrc:/setting/SettingDialog.qml"));


    QHBoxLayout *qhb=new QHBoxLayout(this);
    qhb->setSpacing(0);
    qhb->setContentsMargins(0,0,0,0);
    qhb->addWidget(interface_);

    setMinimumSize(480,580);
    setMaximumSize(480,580);

    setWindowFlag(Qt::Window);
}

void SettingDialog::closeEvent(QCloseEvent *event)
{
    if(changeNotSave_){
        CustomMessageBox w(this,"警告","是否保存修改内容",MsgType::Waring);
        if(w.exec()==QDialog::Accepted){
            QMetaObject::invokeMethod(interface_->rootObject(),"saveSetting");
        }
    }
}

void SettingDialog::showEvent(QShowEvent *)
{
    QMetaObject::invokeMethod(interface_->rootObject(),"init");
}
