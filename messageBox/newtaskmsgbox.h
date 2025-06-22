#ifndef NEWTASKMSGBOX_H
#define NEWTASKMSGBOX_H

#include "base/framless_dialog.h"
#include <QQuickWidget>

class NewTaskMsgBox : public FramlessDialog
{
    Q_OBJECT
public:
    NewTaskMsgBox(QWidget *parent=nullptr);

public slots:
    void saveDownloadInfo(QString url,QString savePosiotion,int thrdNum);

    QString getUrl(){return url_;}
    QString getSavePosition(){return savePosition_;}
    int getThrdNum(){return thrdNum_;}

private:
    QQuickWidget *interface_;

    QString url_;
    QString savePosition_;
    int thrdNum_;
};

#endif // NEWTASKMSGBOX_H
