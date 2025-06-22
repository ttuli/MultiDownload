#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QMimeDatabase>
#include <QNetworkReply>
#include "singledownloadtask.h"

struct FileInfo {
    QString suggestedFileName_;
    qint64 fileSize_ = -1;
    QString contentType_;
    bool isValid_ = false;
    QString errorString_;

    // 辅助方法
    QString formatFileSize() const {
        if (fileSize_ < 0) return "Unknown";

        const qint64 KB = 1024;
        const qint64 MB = KB * 1024;
        const qint64 GB = MB * 1024;

        if (fileSize_ >= GB) {
            return QString::number(fileSize_ / (double)GB, 'f', 2) + " GB";
        } else if (fileSize_ >= MB) {
            return QString::number(fileSize_ / (double)MB, 'f', 2) + " MB";
        } else if (fileSize_ >= KB) {
            return QString::number(fileSize_ / (double)KB, 'f', 2) + " KB";
        } else {
            return QString::number(fileSize_) + " bytes";
        }
    }
};

class SingleDownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit SingleDownloadManager(QObject *parent = nullptr,QString id="",short thrdNum=4
                             ,QUrl url=QUrl()
                             ,QString savePosition="");
public slots:
    void start(QString id);
    void pause(QString id);
    void cancel(QString id);
    QString getId(){return id_;}

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
    QVector<SingleDownloadTask*> thrds_;

signals:
    void finished();
    void errorOccured(QString msg);
};

#endif // DOWNLOADMANAGER_H
