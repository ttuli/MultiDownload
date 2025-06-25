#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QQuickWidget>
#include <QNetworkAccessManager>
#include <QVector>
#include "model/taskmodel.h"
#include "setting/settingdialog.h"
#include "download/downloadmanager.h"

enum class TopMsgPopType{
    Loading=0,
    Success=1,
    Fail=2
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void addTask();
    void cancelTask(QString taskID,int index);
    void pauseTask(QString taskID,int index);
    void startTask(QString taskID,int index);
    void removeTaskFromView(QString taskID,int index);

    void startAllTask();
    void pauseAllTask();
    void cancelAllTask();

    void doSetting();
    void popTopMsg(QString msg,TopMsgPopType type,int duration=1200);
    void removeTask(QString id);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    QQuickWidget *mainInterface_;
    TaskModel *model_;
    SettingDialog *settingDialog_;

    QNetworkAccessManager *manager_;
    QVector<SingleDownloadManager*> tasks_;
};
#endif // WIDGET_H
