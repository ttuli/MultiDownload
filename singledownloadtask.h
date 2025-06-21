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
    explicit SingleDownloadTask(QObject *parent = nullptr,qint64 startByte=0,qint64 endByte=0,QUrl url=QUrl(),QString id="");
    ~SingleDownloadTask();

public slots:
    void startDownload();
    void pauseDownload();
    void cancelDownload();
    void recordDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    QFile file_;

    qint64 startByte_;
    qint64 endByte_;
    QUrl url_;
    QString id_;
    qint64 totalSize_;
    qint64 currentReceiveSize_;

signals:
    void errorOccurred(QString msg);
    void downloadProgress(qint64 bytesReceived,qint64 bytesTotal);
    void cancelSucceeded(QString id);
    void pauseSucceeded(QString id);
    void finished();
};

#endif // SINGLEDOWNLOADTASK_H
