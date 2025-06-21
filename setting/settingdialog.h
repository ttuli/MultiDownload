#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QWidget>
#include <QQuickWidget>

class SettingDialog : public QWidget
{
    Q_OBJECT
public:
    SettingDialog(QWidget *parent=nullptr);

public slots:
    void onConfirm();
    void onReject();
    void setConfig(QVariantMap &config);

private:
    QQuickWidget *interface_;

    static QString configName;
private:
    void readConfig();
};

#endif // SETTINGDIALOG_H
