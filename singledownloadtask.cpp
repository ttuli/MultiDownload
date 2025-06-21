#include "singledownloadtask.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent>
#include <QStandardPaths>

SingleDownloadTask::SingleDownloadTask(QObject *parent,qint64 startByte,qint64 endByte,QUrl url,QString id)
    : QObject{parent},startByte_(startByte),endByte_(endByte),url_(url),id_(id)
{
    manager_=new QNetworkAccessManager(this);
    reply_=nullptr;
    file_.setFileName("part_"+id+".tmp");
    file_.open(QIODevice::WriteOnly);
    currentReceiveSize_=0;
    totalSize_=0;
}

SingleDownloadTask::~SingleDownloadTask()
{
    if(file_.isOpen())
        file_.close();
}

void SingleDownloadTask::startDownload()
{
    QNetworkRequest request(url_);
    request.setRawHeader("Range",QString("bytes=%1-%2").arg(QString::number(startByte_).arg(QString::number(endByte_))).toUtf8());
    reply_=manager_->get(request);
    connect(reply_,&QNetworkReply::readyRead,[this](){
        if(!file_.isOpen()){
            qErrnoWarning("file_未打开文件");
            return;
        }
        file_.write(reply_->readAll());
    });
    connect(reply_,&QNetworkReply::errorOccurred,[this]{
        emit errorOccurred(reply_->errorString());
    });
    connect(reply_,&QNetworkReply::finished,this,[this](){
        emit finished();

        if(reply_->error()!=QNetworkReply::NoError){
            if(reply_->error()==QNetworkReply::OperationCanceledError){
                if(file_.isOpen()){
                    file_.close();
                    file_.remove();
                }
                reply_->deleteLater();
                return;
            }
            emit errorOccurred(reply_->errorString());
        }

        if(file_.isOpen())
            file_.close();
        reply_->abort();
        reply_->deleteLater();
    });
    connect(reply_,&QNetworkReply::destroyed,[]{
        qDebug()<<"QNetworkreply successfully";
    });
    connect(reply_,&QNetworkReply::downloadProgress,this,&SingleDownloadTask::recordDownloadProgress);
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
    currentReceiveSize_=bytesReceived;
    totalSize_=bytesTotal;
    emit downloadProgress(bytesReceived,bytesTotal);
}
