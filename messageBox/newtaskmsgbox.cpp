#include "newtaskmsgbox.h"
#include <QQuickWidget>
#include <QHBoxLayout>
#include <QQmlContext>

NewTaskMsgBox::NewTaskMsgBox(QWidget *parent)
    :FramlessDialog(parent)
{
    interface_=new QQuickWidget(this);
    interface_->setResizeMode(QQuickWidget::SizeRootObjectToView);
    interface_->setAttribute(Qt::WA_AlwaysStackOnTop);
    interface_->rootContext()->setContextProperty("rootWidget",this);
    interface_->setSource(QUrl("qrc:/messageBox/NewTaskBox.qml"));

    QHBoxLayout *qhb=new QHBoxLayout(this);
    qhb->setSpacing(0);
    qhb->setContentsMargins(0,0,0,0);
    qhb->addWidget(interface_);

    setMaximumSize(480,390);
    setMinimumSize(480,390);
}

void NewTaskMsgBox::saveDownloadInfo(QString url, QString savePosiotion,int thrdNum)
{
    url_=url;
    savePosition_=savePosiotion;
    thrdNum_=thrdNum;
    qDebug()<<thrdNum;
}
