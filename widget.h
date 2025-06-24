#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QQuickWidget>
#include <QNetworkAccessManager>
#include <QVector>
#include "model/taskmodel.h"
#include "setting/settingdialog.h"
#include "download/downloadmanager.h"

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
    void deleteTask(QString taskID,int index);
    void startTask(QString taskID,int index);

    void startAllTask();
    void pauseAllTask();
    void cancelAllTask();

    void doSetting();

protected:
    void closeEvent(QCloseEvent*) override;

private:
    QQuickWidget *mainInterface_;
    TaskModel *model_;
    SettingDialog *settingDialog_;

    QNetworkAccessManager *manager_;
    QVector<SingleDownloadManager*> tasks_;
};
#endif // WIDGET_H
