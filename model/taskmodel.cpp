#include "taskmodel.h"

TaskModel::TaskModel(QObject *parent)
    : QAbstractListModel(parent)
{
    SingleTask t(1,"1.txt","c:/download","http://5555",6666);
    t.speed_="1.5mb/s";
    dataList_<<t;
}

int TaskModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return dataList_.size();
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role==SingleTask::FILENAME)return dataList_[index.row()].fileName_;
    if(role==SingleTask::ID)return dataList_[index.row()].id_;
    if(role==SingleTask::PROGRESS)return dataList_[index.row()].progress_;
    if(role==SingleTask::URL)return dataList_[index.row()].url_;
    if(role==SingleTask::STATUS)return kMsgTypeToString.at(dataList_[index.row()].status_);
    if(role==SingleTask::SAVEPOSITION)return dataList_[index.row()].savePosition_;
    if(role==SingleTask::SPEED)return dataList_[index.row()].speed_;
    if(role==SingleTask::SIZE)return dataList_[index.row()].fileSize_;

    return QVariant();
}

bool TaskModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    endInsertRows();
    return true;
}

bool TaskModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);


    endRemoveRows();
    return true;
}

QHash<int, QByteArray> TaskModel::roleNames() const
{
    QHash<int, QByteArray> role;
    role[SingleTask::ID]="model_id";
    role[SingleTask::FILENAME]="model_filename";
    role[SingleTask::URL]="model_url";
    role[SingleTask::PROGRESS]="model_progress";
    role[SingleTask::SAVEPOSITION]="model_saveposition";
    role[SingleTask::SPEED]="model_speed";
    role[SingleTask::STATUS]="model_status";
    role[SingleTask::SIZE]="model_size";

    return role;
}
