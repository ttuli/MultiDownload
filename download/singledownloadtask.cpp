#include "singledownloadtask.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent>
#include <QStandardPaths>

SingleDownloadTask::SingleDownloadTask(QObject *parent,qint64 startByte,qint64 endByte,QUrl url,QString id,int index)
    : QObject{parent},startByte_(startByte),endByte_(endByte),url_(url),id_(id),index_(index)
{
    manager_=new QNetworkAccessManager(this);
    reply_=nullptr;
    file_.setFileName(id+".tmp");
    file_.open(QIODevice::WriteOnly);
    currentReceiveSize_=0;
    totalSize_=0;
}

SingleDownloadTask::~SingleDownloadTask()
{
    if(file_.isOpen())
        file_.close();
    reply_->abort();
    delete reply_;
}

void SingleDownloadTask::startDownload()
{
    QNetworkRequest request(url_);
    request.setRawHeader("Range",QString("bytes=%1-%2").arg(QString::number(startByte_)).arg(QString::number(endByte_)).toUtf8());
    reply_=manager_->get(request);
    connect(reply_,&QNetworkReply::readyRead,[this](){
        if(!file_.isOpen()){
            qErrnoWarning("file_未打开文件");
            return;
        }
        file_.write(reply_->readAll());
    });
    connect(reply_,&QNetworkReply::finished,this,[this](){
        if(file_.isOpen())
            file_.close();
        reply_->abort();
        if(reply_->error()!=QNetworkReply::NoError){
            if(reply_->error()==QNetworkReply::OperationCanceledError){
                if(file_.isOpen()){
                    file_.close();
                    file_.remove();
                }
                reply_->deleteLater();
            } else {
               emit errorOccurred(reply_->errorString());
            }
            return;
        }

        emit finished();

        reply_->deleteLater();
    },Qt::DirectConnection);
    connect(reply_,&QNetworkReply::destroyed,[]{
        qDebug()<<"SingleDownloadTask QNetworkreply successfully";
    });
    connect(reply_,&QNetworkReply::downloadProgress,this,&SingleDownloadTask::recordDownloadProgress,Qt::DirectConnection);
}

void SingleDownloadTask::pauseDownload()
{
    //TODO实现暂停逻辑
    if(reply_!=nullptr){
        reply_->abort();
    }
    emit pauseSucceeded(id_);
}

void SingleDownloadTask::cancelDownload()
{
    if(reply_!=nullptr){
        reply_->abort();
    }
    if(file_.isOpen())
        file_.remove();
    emit cancelSucceeded(id_);
}

void SingleDownloadTask::recordDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug()<<currentReceiveSize_;
    currentReceiveSize_=bytesReceived;
    totalSize_=bytesTotal;
    emit downloadProgress(bytesReceived,bytesTotal);
}
