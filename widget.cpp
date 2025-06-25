#include "widget.h"
#include "messageBox/newtaskmsgbox.h"
#include "messageBox/custommessagebox.h"
#include "download/downloadmanager.h"
#include "setting/settingdialog.h"
#include "setting/settingstruct.h"
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
#include <QThreadPool>

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
    QThreadPool::globalInstance()->setMaxThreadCount(32);
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
        connect(dm,&SingleDownloadManager::errorOccured,this,[this,dm](QString msg){
            CustomMessageBox box(this,"错误",msg,MsgType::Error);
            box.exec();
            model_->updateRow(SingleTask::TaskProperties::STATUS,dm->getId(),QVariant((int)DownloadStatus::ERROR));
            removeTask(dm->getId());
        },Qt::QueuedConnection);
        connect(dm,&SingleDownloadManager::parseFileInfo,this,[this,dm](FileInfo info){
            model_->addNewRow(SingleTask(dm->getId(),info.suggestedFileName_,dm->getSavePosition()
                                         ,dm->getUrl().toString()
                                         ,info.fileSize_),model_->rowCount());
            if(tasks_.size()<=SettingStruct::getInstance()->maxConcurrent())
            dm->createThrd();
        });
        connect(dm,&SingleDownloadManager::downloadProgress,this,[this,dm](qint64 bytesReceived,qint64 bytesTotal){
            double progress=bytesReceived/1.0/bytesTotal;
            double rounded = (int)(progress*100);
            model_->updateRow(SingleTask::TaskProperties::PROGRESS,dm->getId(),QVariant(rounded));
            model_->updateRow(SingleTask::TaskProperties::SPEED,dm->getId(),QVariant(bytesReceived));
            if(bytesReceived==bytesTotal){
                model_->updateRow(SingleTask::TaskProperties::STATUS,dm->getId(),QVariant((int)DownloadStatus::MERGING));
            }
        });
        connect(dm,&SingleDownloadManager::destroyed,[]{
            qDebug()<<"SingleDownloadManager destory";
        });
        connect(dm,&SingleDownloadManager::pauseSuccessed,[this,dm]{
            model_->updateRow(SingleTask::TaskProperties::STATUS,dm->getId(),QVariant((int)DownloadStatus::PAUSED));
        });
        connect(dm,&SingleDownloadManager::restartSuccessed,[this,dm]{
            model_->updateRow(SingleTask::TaskProperties::STATUS,dm->getId(),QVariant((int)DownloadStatus::DOWNLOADING));
        });
        connect(dm,&SingleDownloadManager::cancelSuccessed,[this,dm]{
            removeTask(dm->getId());
            popTopMsg("取消成功",TopMsgPopType::Success);
        });
        connect(dm,&SingleDownloadManager::finished,[this,dm]{
            model_->updateRow(SingleTask::TaskProperties::STATUS,dm->getId(),QVariant((int)DownloadStatus::FINISHED));
            removeTask(dm->getId());
        });
        tasks_.append(dm);
        dm->start();
    }
}

void Widget::cancelTask(QString taskID,int index)
{
    if(tasks_.value(index,nullptr)&&tasks_[index]->getId()==taskID){
        tasks_[index]->cancel();
    } else {
        qDebug()<<"Widget tasks_.value nullptr";
    }
}

void Widget::pauseTask(QString taskID,int index)
{
    if(tasks_.value(index,nullptr)&&tasks_[index]->getId()==taskID){
        tasks_[index]->pause();
    } else {
        qDebug()<<"Widget tasks_.value nullptr";
    }
}

void Widget::startTask(QString taskID, int index)
{
    if(tasks_.value(index,nullptr)&&tasks_[index]->getId()==taskID){
        tasks_[index]->restart();
    } else {
        qDebug()<<"Widget tasks_.value nullptr";
    }
}

void Widget::removeTaskFromView(QString taskID, int index)
{
    if(tasks_.value(index,nullptr)&&tasks_[index]->getId()==taskID){
        if(tasks_[index]->isRunning()){
            CustomMessageBox w(this,"警告","任务正在进行，确定删除吗？",MsgType::Waring);
            if(w.exec()==QDialog::Rejected)
                return;
        }
    } else {
        qDebug()<<"Widget tasks_.value nullptr";
    }
    removeTask(taskID);
    model_->removeARow(taskID);
}

void Widget::startAllTask()
{
    for(auto i:tasks_)
        i->restart();
}

void Widget::pauseAllTask()
{
    for(auto i:tasks_)
        i->pause();
}

void Widget::cancelAllTask()
{
    CustomMessageBox w(this,"警告","确定取消全部任务?",MsgType::Waring);
    if(w.exec()!=QDialog::Accepted)
        return;
    int size=tasks_.size();
    for(int i=0;i<tasks_.size();++i){
        cancelTask(tasks_.at(i)->getId(),i);
        model_->removeARow(0);
    }  
}

void Widget::doSetting()
{
    if(settingDialog_!=nullptr){
        settingDialog_->show();
        settingDialog_->activateWindow();
        return;
    }
    settingDialog_=new SettingDialog(this);
    connect(settingDialog_,&SettingDialog::destroyed,[this]{
        settingDialog_=nullptr;
    });
    settingDialog_->show();
}

void Widget::popTopMsg(QString msg, TopMsgPopType type, int duration)
{
    QMetaObject::invokeMethod(mainInterface_->rootObject(),"showMsg"
                              ,Q_ARG(QVariant,msg)
                              ,Q_ARG(QVariant,(int)type)
                              ,Q_ARG(QVariant,duration));
}

void Widget::removeTask(QString id)
{
    for(int i=0;i<tasks_.size();++i){
        if(tasks_[i]->getId()==id){
            tasks_[i]->close();
            tasks_[i]->deleteLater();
            tasks_.removeAt(i);
            break;
        }
    }
    for(int i=0;i<tasks_.size()&&i<SettingStruct::getInstance()->maxConcurrent();++i){
        if(!tasks_[i]->isRunning())
            tasks_[i]->createThrd();
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(tasks_.size()){
        CustomMessageBox w(this,"警告","有正在执行的任务，确定退出吗?",MsgType::Waring);
        if(w.exec()!=QDialog::Accepted){
            event->ignore();
            return;
        }
    }
    for(auto i:tasks_){
        i->close();
        i->deleteLater();
    }
}
