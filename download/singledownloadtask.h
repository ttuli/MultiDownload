#ifndef SINGLEDOWNLOADTASK_H
#define SINGLEDOWNLOADTASK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class SingleDownloadTask : public QObject
{
    Q_OBJECT
public:
    explicit SingleDownloadTask(QObject *parent = nullptr,qint64 startByte=0,qint64 endByte=0,QUrl url=QUrl(),
                                QString id="",int index=-1,bool &cancel=constValue);
    ~SingleDownloadTask();

public slots:
    void startDownload();
    void pauseDownload();
    void cancelDownload();
    void recordDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void restartDownload();

    QString getId(){return id_;}
    int getIndex(){return index_;}

    void close();

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    QFile file_;

    qint64 constStartByte_;//记录最初的
    qint64 startByte_;
    qint64 endByte_;
    QUrl url_;
    QString id_;
    qint64 totalSize_;
    qint64 currentReceiveSize_;
    qint64 preReceiveSize_;
    qint64 currentFileSize_;
    int index_;
    bool &cancel_;
    static bool constValue;

signals:
    void errorOccurred(QString msg);
    void downloadProgress(int index,qint64 bytesReceived,qint64 bytesTotal);
    void cancelSucceeded(QString id);
    void pauseSucceeded(QString id);
    void startSucceeded(QString id);
    void finished(QString id);
};

#endif // SINGLEDOWNLOADTASK_H
