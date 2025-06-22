#include "downloadmanager.h"
#include <QMimeType>
#include <QRegularExpressionMatch>
#include <QRegularExpression>
#include <QtConcurrent>

SingleDownloadManager::SingleDownloadManager(QObject *parent,QString id
                                 ,short thrdNum,QUrl url,QString savePosition)
    : QObject{parent},id_(id),thrdNum_(thrdNum),url_(url),savePosition_(savePosition)
{
    manager_=new QNetworkAccessManager(this);
}

void SingleDownloadManager::start(QString id)
{
    if(id!="all"&&id_!=id)
        return;
    QNetworkRequest request(url_);
    QNetworkReply *reply=manager_->head(request);
    connect(reply,&QNetworkReply::finished,[this,reply]{
        if(reply->error()!=QNetworkReply::NoError){
            emit errorOccured(reply->errorString());
            reply->deleteLater();
            return;
        }
        extractFileInfo(reply);
    });
}

void SingleDownloadManager::pause(QString id)
{
    if(id!="all"&&id_!=id)
        return;
}

void SingleDownloadManager::cancel(QString id)
{
    if(id!="all"&&id_!=id)
        return;
}

void SingleDownloadManager::createThrd(FileInfo info)
{
    info_=info;
    qint64 interval=info.fileSize_/thrdNum_;
    qint64 l=0;
    qint64 r=-1;
    for(int i=1;i<=thrdNum_;++i){
        l=r+1;
        r=i*interval;
        if(i==thrdNum_)
            r=info.fileSize_-1;

        QtConcurrent::run([this]{
            SingleDownloadTask *task=new SingleDownloadTask(this,l,r,url_,id_+QString::number(i));
            task->startDownload();
        });
    }

}


void SingleDownloadManager::extractFileInfo(QNetworkReply *reply)
{
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpStatus < 200 || httpStatus >= 300) {
        emit errorOccured(QString("HTTP Error: %1").arg(httpStatus));
        reply->deleteLater();
        return;
    }

    FileInfo info;

    bool hasContentLength = false;
    qint64 contentLength = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong(&hasContentLength);
    if (hasContentLength && contentLength > 0) {
        info.fileSize_ = contentLength;
    } else {
        // 尝试从原始头部获取
        QByteArray lengthHeader = reply->rawHeader("Content-Length");
        if (!lengthHeader.isEmpty()) {
            info.fileSize_ = lengthHeader.toLongLong();
        }
    }
    if(info.fileSize_==0){
        emit errorOccured("获取文件信息失败");
        return;
    }

    // 2. 获取内容类型
    info.contentType_ = reply->header(QNetworkRequest::ContentTypeHeader).toString();

    // 3. 获取建议文件名（按优先级）
    QString suggestedName;

    // 优先级1: Content-Disposition头部
    QString contentDisposition = reply->rawHeader("Content-Disposition");
    if (!contentDisposition.isEmpty()) {
        suggestedName = extractFileNameFromContentDisposition(contentDisposition);
        qDebug() << "Filename from Content-Disposition:" << suggestedName;
    }

    // 优先级2: 从URL提取
    if (suggestedName.isEmpty()) {
        suggestedName = extractFileNameFromUrl(reply->url());
        qDebug() << "Filename from URL:" << suggestedName;
    }

    // 优先级3: 从Content-Type推断
    if (suggestedName.isEmpty()) {
        suggestedName = extractFileNameFromContentType(info.contentType_);
        qDebug() << "Filename from Content-Type:" << suggestedName;
    }


    info.suggestedFileName_ = sanitizeFileName(suggestedName);
    info.isValid_ = true;
    reply->deleteLater();
    createThrd(info);
}

QString SingleDownloadManager::extractFileNameFromUrl(const QUrl &url)
{
    QString path = url.path();

    // 移除查询参数和片段
    if (path.contains('?')) {
        path = path.left(path.indexOf('?'));
    }
    if (path.contains('#')) {
        path = path.left(path.indexOf('#'));
    }

    // 获取路径的最后一部分
    QStringList pathParts = path.split('/', Qt::SkipEmptyParts);
    if (!pathParts.isEmpty()) {
        QString fileName = pathParts.last();
        // URL解码
        fileName = QUrl::fromPercentEncoding(fileName.toUtf8());
        if (!fileName.isEmpty()) {
            return fileName;
        }
    }

    return "";
}

QString SingleDownloadManager::extractFileNameFromContentDisposition(const QString &contentDisposition)
{
    QRegularExpression filenameRegex;
    QString fileName;

    // 1. 尝试 filename*= (RFC 6266, 支持编码)
    filenameRegex.setPattern(R"(filename\*\s*=\s*(?:UTF-8''|iso-8859-1'')?([^;]+))");
    QRegularExpressionMatch match = filenameRegex.match(contentDisposition);
    if (match.hasMatch()) {
        fileName = QUrl::fromPercentEncoding(match.captured(1).toUtf8());
        return fileName.remove('"').trimmed();
    }

    // 2. 尝试 filename= (传统方式)
    filenameRegex.setPattern(R"(filename\s*=\s*(?:\"([^"]+)\"|([^ ;]+)))");
    match = filenameRegex.match(contentDisposition);
    if (match.hasMatch()) {
        fileName = match.captured(1);
        return fileName.remove('"').trimmed();
    }

    return QString();
}

QString SingleDownloadManager::extractFileNameFromContentType(const QString &contentType)
{
    if (contentType.isEmpty()) {
        return QString();
    }

    // 从MIME类型获取默认扩展名
    QString mimeType = contentType.split(';').first().trimmed();
    QMimeType mime = m_mimeDatabase.mimeTypeForName(mimeType);

    if (mime.isValid() && !mime.suffixes().isEmpty()) {
        return "download." + mime.suffixes().first();
    }

    // 常见类型的硬编码映射
    static QHash<QString, QString> mimeToExt = {
        {"text/html", "download.html"},
        {"text/plain", "download.txt"},
        {"application/pdf", "download.pdf"},
        {"application/zip", "download.zip"},
        {"application/json", "download.json"},
        {"image/jpeg", "download.jpg"},
        {"image/png", "download.png"},
        {"image/gif", "download.gif"},
        {"video/mp4", "download.mp4"},
        {"audio/mpeg", "download.mp3"}
    };

    return mimeToExt.value(mimeType, QString());
}

QString SingleDownloadManager::sanitizeFileName(const QString &fileName)
{
    QString sanitized = fileName;

    // 移除或替换不安全的字符
    static QRegularExpression unsafeChars(R"([<>:"/\\|?*])");
    sanitized.replace(unsafeChars, "_");

    // 移除控制字符
    sanitized.remove(QRegularExpression(R"([\x00-\x1F\x7F])"));

    // 限制长度
    if (sanitized.length() > 255) {
        QString ext;
        int dotIndex = sanitized.lastIndexOf('.');
        if (dotIndex > 0) {
            ext = sanitized.mid(dotIndex);
            sanitized = sanitized.left(255 - ext.length()) + ext;
        } else {
            sanitized = sanitized.left(255);
        }
    }

    // 移除首尾空格和点
    sanitized = sanitized.trimmed();
    while (sanitized.endsWith('.')) {
        sanitized.chop(1);
    }

    // 确保不为空
    if (sanitized.isEmpty()) {
        sanitized = "download";
    }

    return sanitized;
}
