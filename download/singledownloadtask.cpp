#include "singledownloadtask.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent>
#include <QStandardPaths>

bool SingleDownloadTask::constValue=false;

SingleDownloadTask::SingleDownloadTask(QObject *parent,qint64 startByte,qint64 endByte,QUrl url,QString id,int index,bool &cancel)
    : QObject{parent},startByte_(startByte),endByte_(endByte),url_(url),id_(id),index_(index),cancel_(cancel)
{
    manager_=new QNetworkAccessManager(this);
    reply_=nullptr;
    file_.setFileName(id+".tmp");
    file_.open(QIODevice::Append);
    currentReceiveSize_=0;
    currentFileSize_=0;
    preReceiveSize_=0;
    totalSize_=endByte-startByte+1;
    constStartByte_=startByte;
}

SingleDownloadTask::~SingleDownloadTask()
{

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
        currentFileSize_=file_.size();
    });
    connect(reply_,&QNetworkReply::finished,this,[this](){
        if(reply_->error()!=QNetworkReply::NoError){
            if(reply_->error()!=QNetworkReply::OperationCanceledError){
                emit errorOccurred(reply_->errorString());
            }
            return;
        }
        qDebug()<<"reply finish 调用close()";
        close();
    },Qt::DirectConnection);
    connect(reply_,&QNetworkReply::destroyed,[]{
        qDebug()<<"SingleDownloadTask QNetworkreply successfully";
    });
    connect(reply_,&QNetworkReply::downloadProgress,this,&SingleDownloadTask::recordDownloadProgress,Qt::DirectConnection);
}

void SingleDownloadTask::pauseDownload()
{
    qDebug()<<__FUNCTION__;
    if(reply_!=nullptr){
        reply_->abort();
        reply_->deleteLater();
        reply_=nullptr;
    }
    preReceiveSize_=currentReceiveSize_;
    emit pauseSucceeded(id_);
}

void SingleDownloadTask::cancelDownload()
{
    close();
    emit cancelSucceeded(id_);
}

void SingleDownloadTask::recordDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    currentReceiveSize_=preReceiveSize_+bytesReceived;
    emit downloadProgress(index_,currentReceiveSize_,totalSize_);
}

void SingleDownloadTask::restartDownload()
{
    qint64 beginbyte=constStartByte_+currentFileSize_;
    startByte_=beginbyte;
    startDownload();
    emit startSucceeded(id_);
}

void SingleDownloadTask::close()
{
    qDebug()<<"SingleDownloadTask::close";
    if(cancel_){
        file_.remove();
    }
    file_.close();
    if(reply_){
        reply_->abort();
        reply_->deleteLater();
        reply_=nullptr;
    }
    emit finished(id_);
    deleteLater();
}
