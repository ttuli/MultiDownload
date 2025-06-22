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
    void cancelTask(QString taskID);
    void pauseTask(QString taskID);
    void deleteTask(QString taskID);

    void doSetting();

private:
    QQuickWidget *mainInterface_;
    TaskModel *model_;
    SettingDialog *settingDialog_;

    QNetworkAccessManager *manager_;
    QVector<SingleDownloadManager*> tasks_;
};
#endif // WIDGET_H
