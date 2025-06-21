#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractListModel>
#include <QString>

enum class DownloadStatus{
    WAITTING,
    DOWNLOADING,
    FINISHED,
    CANCELED,
    PAUSED,
};

class SingleTask{
public:
    enum TaskProperties{
        ID=0,
        FILENAME,
        PROGRESS,
        SPEED,
        URL,
        STATUS,
        SAVEPOSITION,
        SIZE
    };
    SingleTask(int id,QString fileName,QString savePosition,QString url,qint64 fileSize)
        :id_(id),fileName_(fileName),url_(url),savePosition_(savePosition),fileSize_(fileSize)
    {
        progress_=0;
        speed_="";
        status_=DownloadStatus::WAITTING;
    }

    int id_;
    QString fileName_;
    double progress_;
    QString speed_;
    QString url_;
    DownloadStatus status_;
    QString savePosition_;
    qint64 fileSize_;
};

class TaskModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TaskModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QHash<int,QByteArray> roleNames() const override;
private:
    QVector<SingleTask> dataList_;

    const std::unordered_map<DownloadStatus, QString> kMsgTypeToString = {
        {DownloadStatus::DOWNLOADING, "downloading"},
        {DownloadStatus::PAUSED, "paused"},
        {DownloadStatus::CANCELED, "cancel"},
        {DownloadStatus::FINISHED, "success"},
        {DownloadStatus::WAITTING,"waitting"}
    };
};

#endif // TASKMODEL_H
