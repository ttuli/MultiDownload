#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QMimeDatabase>
#include <QNetworkReply>
#include <QMutex>
#include "singledownloadtask.h"

struct FileInfo {
    QString suggestedFileName_;
    qint64 fileSize_ = -1;
    QString contentType_;
    bool isValid_ = false;
    QString errorString_;

};

class SingleDownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit SingleDownloadManager(QObject *parent = nullptr,QString id="",short thrdNum=4
                             ,QUrl url=QUrl()
                             ,QString savePosition="");
    ~SingleDownloadManager();
public slots:
    void start(QString id);
    void pause(QString id);
    void cancel(QString id);

    void sumDownloadProgress(qint64 bytesReceived,qint64 bytesTotal);

    QString getId(){return id_;}
    QString getSavePosition(){return savePosition_;}
    QUrl getUrl(){return url_;}


private:
    void extractFileInfo(QNetworkReply *reply);
    QString sanitizeFileName(const QString &fileName);
    QString extractFileNameFromContentType(const QString &contentType);
    QString extractFileNameFromContentDisposition(const QString &contentDisposition);
    QString extractFileNameFromUrl(const QUrl &url);

    void createThrd(FileInfo info);

private:
    QString id_;
    short thrdNum_;
    QUrl url_;
    QString savePosition_;
    FileInfo info_;

    QNetworkAccessManager *manager_;
    QMimeDatabase m_mimeDatabase;

    QMutex mutex_;
    QVector<SingleDownloadTask*> thrds_;
    QVector<double> progress_;

signals:
    void finished();
    void errorOccured(QString msg);
    void downloadProgress(qint64 bytesReceived,qint64 bytesTotal);
    void parseFileInfo(FileInfo info);
};

#endif // DOWNLOADMANAGER_H
