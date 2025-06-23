#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractListModel>
#include <QString>
#include <QDateTime>

enum class DownloadStatus{
    WAITTING,
    DOWNLOADING,
    FINISHED,
    CANCELED,
    PAUSED,
    MERGING,
    ERROR
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
    SingleTask(QString id,QString fileName,QString savePosition,QString url,qint64 fileSize)
        :id_(id),fileName_(fileName),url_(url),savePosition_(savePosition),fileSize_(fileSize)
    {
        progress_=0;
        status_=DownloadStatus::DOWNLOADING;
        beginTime_=QDateTime::currentDateTime();
    }
    SingleTask(){}

    QString id_;
    QString fileName_;
    double progress_;
    qint64 speed_;
    QString url_;
    DownloadStatus status_;
    QString savePosition_;
    qint64 fileSize_;
    QDateTime beginTime_;
};

class TaskModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TaskModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

public slots:
    QString GetName(int index);
    QString GetId(int index);
    bool addNewRow(SingleTask task,int position=0);
    bool removeARow(QString id);
    bool removeARow(int position);
    void updateRow(SingleTask::TaskProperties type,int position,QVariant data);
    void updateRow(SingleTask::TaskProperties type,QString id,QVariant data);

    QString formatFileSize(qint64 fileSize) const;

private:
    QVector<SingleTask> dataList_;

    const std::unordered_map<DownloadStatus, QString> kMsgTypeToString = {
        {DownloadStatus::DOWNLOADING, "downloading"},
        {DownloadStatus::PAUSED, "paused"},
        {DownloadStatus::CANCELED, "cancel"},
        {DownloadStatus::FINISHED, "completed"},
        {DownloadStatus::WAITTING,"waitting"},
        {DownloadStatus::MERGING,"merging"},
        {DownloadStatus::ERROR,"error"}

    };
};

#endif // TASKMODEL_H
