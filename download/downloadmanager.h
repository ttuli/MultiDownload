#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QMimeDatabase>
#include <QNetworkReply>
#include <QMutex>
#include <QTimer>
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
    void start();
    void restart();
    void pause();
    void cancel();
    void createThrd();

    void sumDownloadProgress(int index,qint64 bytesReceived,qint64 bytesTotal);
    void sumCancelNum(QString id);
    void sumPauseNum(QString id);
    void sumStartNum(QString id);

    QString getId(){return id_;}
    QString getSavePosition(){return savePosition_;}
    QUrl getUrl(){return url_;}
    bool isRunning(){return running_;}
    FileInfo getInfo(){return info_;}

    void close();

private:
    void extractFileInfo(QNetworkReply *reply);
    QString sanitizeFileName(const QString &fileName);
    QString extractFileNameFromContentType(const QString &contentType);
    QString extractFileNameFromContentDisposition(const QString &contentDisposition);
    QString extractFileNameFromUrl(const QUrl &url);

    void removeThrd(QString id);
    void mergeFile();
    void clearAllTmpFile();

private:
    QString id_;
    short thrdNum_;
    QUrl url_;
    QString savePosition_;
    FileInfo info_;
    bool cancel_;
    bool running_=false;

    int pauseNum_=0;
    int startNum_=0;
    int cancelNum_=0;
    QTimer *clickInterval_;
    bool clickable_=true;//避免频繁开始，暂停

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
    void pauseSig();
    void pauseSuccessed();
    void restartSig();
    void restartSuccessed();
    void cancelSig();
    void cancelSuccessed();
};

#endif // DOWNLOADMANAGER_H
