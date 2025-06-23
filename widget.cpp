#include "widget.h"
#include "messageBox/newtaskmsgbox.h"
#include "messageBox/custommessagebox.h"
#include "download/downloadmanager.h"
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

Widget::~Widget()
{

}

void Widget::addTask()
{
    NewTaskMsgBox w;
    if(w.exec()==QDialog::Accepted){
        QString url=w.getUrl();
        QString savePosition=w.getSavePosition();
        int thrdNum=w.getThrdNum();
        QRegularExpression re(R"(https?://(?:www\.)?[a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*(?::\d+)?/[^\s]*\.(zip|rar|7z|tar|gz|bz2|xz|exe|msi|dmg|pkg|deb|rpm|apk|jar|war|ear|pdf|doc|docx|xls|xlsx|ppt|pptx|mp3|mp4|avi|mkv|mov|wmv|flv|webm|iso|img|bin)(?:\?[^\s&]*(?:&[^\s&]*)*)?(?:#[^\s]*)?)");
        QRegularExpressionMatch match=re.match(url);

        if(!match.hasMatch()){
            CustomMessageBox box(this,"错误","下载地址有误!",MsgType::Error);
            box.exec();
            return;
        }

        QString id=QUuid::createUuid().toString(QUuid::WithoutBraces);
        SingleDownloadManager *dm=new SingleDownloadManager(this,id,thrdNum,url,savePosition);
        connect(dm,&SingleDownloadManager::errorOccured,[this,dm](QString msg){
            CustomMessageBox box(this,"错误",msg,MsgType::Error);
            box.exec();
            for(int i=0;i<tasks_.size();i++){
                if(tasks_.at(i)->getId()==dm->getId()){
                    tasks_.at(i)->deleteLater();
                    tasks_.removeAt(i);
                }
            }
        });
        connect(dm,&SingleDownloadManager::parseFileInfo,this,[this,dm](FileInfo info){
            model_->addNewRow(SingleTask(dm->getId(),info.suggestedFileName_,dm->getSavePosition()
                                         ,dm->getUrl().toString()
                                         ,info.fileSize_));
        });
        connect(dm,&SingleDownloadManager::downloadProgress,this,[this,dm](qint64 bytesReceived,qint64 bytesTotal){
            double progress=bytesReceived/1.0/bytesTotal;
            double rounded = (int)(progress*100);
            model_->updateRow(SingleTask::TaskProperties::PROGRESS,dm->getId(),QVariant(rounded));
            model_->updateRow(SingleTask::TaskProperties::SPEED,dm->getId(),QVariant(bytesReceived));
            if(bytesReceived==bytesTotal){
                model_->updateRow(SingleTask::TaskProperties::STATUS,dm->getId(),QVariant((int)DownloadStatus::FINISHED));
                for(int i=0;i<tasks_.size();++i){
                    if(tasks_[i]->getId()==dm->getId()){
                        dm->close();
                        dm->deleteLater();
                        tasks_.removeAt(i);
                        break;
                    }
                }
            }
        });
        connect(dm,&SingleDownloadManager::destroyed,[]{
            qDebug()<<"SingleDownloadManager destory";
        });
        connect(dm,&SingleDownloadManager::pauseSuccessed,[this,dm]{
            model_->updateRow(SingleTask::TaskProperties::STATUS,dm->getId(),QVariant((int)DownloadStatus::PAUSED));
        });
        tasks_.append(dm);
        dm->start(id);
    }
}

void Widget::cancelTask(QString taskID,int index)
{

}

void Widget::pauseTask(QString taskID,int index)
{
    if(tasks_.value(index,nullptr)&&tasks_[index]->getId()==taskID){
        tasks_[index]->pause(taskID);
    }
}

void Widget::deleteTask(QString taskID,int index)
{
    if(tasks_.value(index,nullptr)&&tasks_[index]->getId()==taskID){

    }
}

void Widget::startTask(QString taskID, int index)
{
    if(tasks_.value(index,nullptr)&&tasks_[index]->getId()==taskID){

    }
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

void Widget::closeEvent(QCloseEvent *)
{
    for(auto i:tasks_){
        i->close();
        i->deleteLater();
    }
}
