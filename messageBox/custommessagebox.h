#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QDialog>
#include <QQuickWidget>
#include <QQuickItem>
#include "base/framless_dialog.h"

enum class MsgType{
    Info=0,
    Waring,
    Error,
    Success
};

class CustomMessageBox : public FramlessDialog
{
    Q_OBJECT
public:
    CustomMessageBox(QWidget *parent=nullptr,QString caption="提示消息",QString content="",MsgType type=MsgType::Info);

    const std::unordered_map<MsgType, QString> kMsgTypeToString = {
        {MsgType::Info, "info"},
        {MsgType::Waring, "waring"},
        {MsgType::Error, "error"},
        {MsgType::Success, "success"}
    };

public slots:
    void onAccept(){accept();}
    void onReject(){reject();}

private:
    QQuickWidget *interface_;

};

#endif // CUSTOMMESSAGEBOX_H
