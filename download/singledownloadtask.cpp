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
    totalSize_=0;
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
    });
    connect(reply_,&QNetworkReply::finished,this,[this](){
        if(reply_->error()!=QNetworkReply::NoError){
            if(reply_->error()!=QNetworkReply::OperationCanceledError){
                emit errorOccurred(reply_->errorString());
            }
            return;
        }
        close();
    },Qt::DirectConnection);
    connect(reply_,&QNetworkReply::destroyed,[]{
        qDebug()<<"SingleDownloadTask QNetworkreply successfully";
    });
    connect(reply_,&QNetworkReply::downloadProgress,this,&SingleDownloadTask::recordDownloadProgress,Qt::DirectConnection);
}

void SingleDownloadTask::pauseDownload()
{
    if(!reply_->isRunning())
        return;
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
    emit cancelSucceeded(id_);
}

void SingleDownloadTask::recordDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    currentReceiveSize_=bytesReceived;
    totalSize_=bytesTotal;
    emit downloadProgress(bytesReceived,bytesTotal);
}

void SingleDownloadTask::close()
{
    qDebug()<<"SingleDownloadTask::close";
    file_.close();
    if(cancel_)
        file_.remove();
    if(reply_){
        reply_->abort();
        reply_->deleteLater();
        reply_=nullptr;
    }
    emit finished(id_);
    deleteLater();
}
