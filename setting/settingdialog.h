#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QWidget>
#include <QQuickWidget>

class SettingDialog : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool changeNotSave READ changeNotSave WRITE setChangeNotSave NOTIFY changeNotSaveChanged FINAL)
public:
    SettingDialog(QWidget *parent=nullptr);

public slots:
    bool changeNotSave(){return changeNotSave_;}
    void setChangeNotSave(bool val){
        changeNotSave_=val;
        emit changeNotSaveChanged();
    }

protected:
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent*) override;

private:
    QQuickWidget *interface_;

    bool changeNotSave_=false;
signals:
    void changeNotSaveChanged();
};

#endif // SETTINGDIALOG_H
