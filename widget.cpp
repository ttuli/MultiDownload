#include "widget.h"
#include "messageBox/newtaskmsgbox.h"
#include "messageBox/custommessagebox.h"
#include "singledownloadtask.h"
#include "setting/settingdialog.h"
#include <QHBoxLayout>
#include <QQmlContext>
#include <QQuickItem>
#include <QtConcurrent>
#include <QRegularExpression>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QUuid>
#include <model/taskmodel.h>
#include <QEventLoop>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    model_=new TaskModel(this);
    mainInterface_=new QQuickWidget(this);
    mainInterface_->rootContext()->setContextProperty("taskModel",model_);
    mainInterface_->rootContext()->setContextProperty("rootWidget",this);
    mainInterface_->rootContext()->setContextProperty("DownloadStatus::WAITTING",(int)DownloadStatus::WAITTING);
    mainInterface_->rootContext()->setContextProperty("DownloadStatus::DOWNLOADING",(int)DownloadStatus::DOWNLOADING);
    mainInterface_->rootContext()->setContextProperty("DownloadStatus::CANCELED",(int)DownloadStatus::CANCELED);
    mainInterface_->rootContext()->setContextProperty("DownloadStatus::FINISHED",(int)DownloadStatus::FINISHED);
    mainInterface_->rootContext()->setContextProperty("DownloadStatus::PAUSED",(int)DownloadStatus::PAUSED);
    mainInterface_->setSource(QUrl("qrc:/MainInterface.qml"));
    mainInterface_->setResizeMode(QQuickWidget::SizeRootObjectToView);

    manager_=new QNetworkAccessManager(this);

    QHBoxLayout *qhb=new QHBoxLayout(this);
    qhb->setContentsMargins(0,0,0,0);
    qhb->setSpacing(0);
    qhb->addWidget(mainInterface_);

    settingDialog_=nullptr;

    setMinimumSize(640,480);
}

Widget::~Widget() {}

void Widget::addTask()
{
    NewTaskMsgBox w;
    if(w.exec()==QDialog::Accepted){
        QString url=w.getUrl();
        QString savePosition=w.getSavePosition();
        QRegularExpression re(R"(https?:\/\/[^\s\/]+(?:\/[^\s\/]+)*\/[^\/\s]+\.(zip|rar|exe|tar\.gz|7z|pdf|docx?|xlsx?|apk|mp[34]|iso))");
        QRegularExpressionMatch match=re.match(url);

        if(!match.hasMatch()){
            CustomMessageBox box(this,"错误","下载地址有误!",MsgType::Error);
            box.exec();
            return;
        }

        getFileInfo(url);
    }
}

void Widget::cancelTask(qint64 taskID)
{

}

void Widget::pauseTask(qint64 taskID)
{

}

void Widget::deleteTask(qint64 taskID)
{

}

void Widget::getFileInfo(QUrl url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply=manager_->head(request);
    connect(reply,&QNetworkReply::finished,[this,reply,url]{
        reply->deleteLater();
        startTask(url);
    });
}

void Widget::startTask(QUrl url)
{
    QString id=QUuid::createUuid().toString(QUuid::WithoutBraces);
    QtConcurrent::run([this]{
        SingleDownloadTask task;
        QEventLoop loop;
        connect(&task,&SingleDownloadTask::finished,&loop,&QEventLoop::quit);
        loop.exec();
    });
}

void Widget::doSetting()
{
    if(settingDialog_!=nullptr){
        settingDialog_->activateWindow();
        return;
    }
    settingDialog_=new SettingDialog(this);
    connect(settingDialog_,&SettingDialog::destroyed,[this]{
        settingDialog_=nullptr;
    });
    settingDialog_->show();
}
