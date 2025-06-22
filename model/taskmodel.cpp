#include "taskmodel.h"

TaskModel::TaskModel(QObject *parent)
    : QAbstractListModel(parent)
{

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
    if(role==SingleTask::SPEED)return formatFileSize(dataList_[index.row()].speed_);
    if(role==SingleTask::SIZE)return formatFileSize(dataList_[index.row()].fileSize_);

    return QVariant();
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

QString TaskModel::GetName(int index)
{
    if(index<0||index>=dataList_.size()){
        return "";
    }
    return dataList_.at(index).fileName_;
}

bool TaskModel::addNewRow(SingleTask task,int position)
{
    if(position<0||position>rowCount()){
        qDebug()<<__FUNCTION__<<"invalid index";
        return false;
    }
    beginInsertRows(QModelIndex(),position,position);
    dataList_.insert(position,task);
    endInsertRows();
    return true;
}

bool TaskModel::removeARow(QString id)
{
    for(int i=0;i<dataList_.size();++i){
        if(dataList_.at(i).id_==id){
            beginRemoveRows(QModelIndex(),i,i);
            dataList_.removeAt(i);
            endRemoveRows();
            return true;
        }
    }
    return false;
}

bool TaskModel::removeARow(int position)
{
    if(position<0||position>=rowCount()){
        qDebug()<<__FUNCTION__<<"invalid index";
        return false;
    }
    beginRemoveRows(QModelIndex(),position,position);
    dataList_.removeAt(position);
    endRemoveRows();
    return true;
}

void TaskModel::updateRow(SingleTask::TaskProperties type, int position, QVariant data)
{
    if(!data.isValid()){
        qDebug()<<__FUNCTION__<<"invalid data";
        return;
    }
    if(position<0||position>=rowCount()){
        qDebug()<<__FUNCTION__<<"invalid index";
        return;
    }
    if(type==SingleTask::FILENAME)dataList_[position].fileName_=data.toString();
    if(type==SingleTask::PROGRESS)dataList_[position].progress_=data.toDouble();
    if(type==SingleTask::STATUS)dataList_[position].status_=(DownloadStatus)data.toInt();
    if(type==SingleTask::SPEED){
        qint64 num=data.toULongLong();
        dataList_[position].speed_=num/1.0/qAbs(QDateTime::currentDateTime().secsTo(dataList_[position].beginTime_));
    }
    emit dataChanged(index(position),index(position),{type});
}

void TaskModel::updateRow(SingleTask::TaskProperties type, QString id, QVariant data)
{
    for(int i=0;i<dataList_.size();i++){
        if(dataList_.at(i).id_==id){
            updateRow(type,i,data);
            return;
        }
    }
    qDebug()<<__FUNCTION__<<"invalid id";
}

QString TaskModel::formatFileSize(qint64 fileSize) const {
    if (fileSize < 0) return "Unknown";

    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;

    if (fileSize >= GB) {
        return QString::number(fileSize / (double)GB, 'f', 2) + " GB";
    } else if (fileSize >= MB) {
        return QString::number(fileSize / (double)MB, 'f', 2) + " MB";
    } else if (fileSize >= KB) {
        return QString::number(fileSize / (double)KB, 'f', 2) + " KB";
    } else {
        return QString::number(fileSize) + " bytes";
    }

    return "";
}
