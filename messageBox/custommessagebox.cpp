#include "custommessagebox.h"
#include <QHBoxLayout>
#include <QQmlContext>

CustomMessageBox::CustomMessageBox(QWidget *parent,QString caption,QString content,MsgType type)
    :FramlessDialog(parent)
{
    interface_=new QQuickWidget(this);
    interface_->setAttribute(Qt::WA_AlwaysStackOnTop);
    interface_->setResizeMode(QQuickWidget::SizeRootObjectToView);
    interface_->rootContext()->setContextProperty("rootWidget",this);
    interface_->setSource(QUrl("qrc:/messageBox/MessageBox.qml"));

    QMetaObject::invokeMethod(interface_->rootObject(),"setTitleAContent"
                              ,Q_ARG(QVariant,caption)
                              ,Q_ARG(QVariant,content)
                              ,Q_ARG(QVariant,kMsgTypeToString.at(type)));

    QHBoxLayout *qhb=new QHBoxLayout(this);
    qhb->setContentsMargins(0,0,0,0);
    qhb->setSpacing(0);
    qhb->addWidget(interface_);

    setMaximumSize(400,250);
    setMinimumSize(400,250);
}
