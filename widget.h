#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QQuickWidget>
#include <QNetworkAccessManager>
#include "model/taskmodel.h"
#include "setting/settingdialog.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void addTask();
    void cancelTask(qint64 taskID);
    void pauseTask(qint64 taskID);
    void deleteTask(qint64 taskID);
    void getFileInfo(QUrl url);
    void startTask(QUrl url);

    void doSetting();

private:
    QQuickWidget *mainInterface_;
    TaskModel *model_;
    SettingDialog *settingDialog_;

    QNetworkAccessManager *manager_;
};
#endif // WIDGET_H
