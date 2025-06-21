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
    void saveDownloadInfo(QString url,QString savePosiotion);

    QString getUrl(){return url_;}
    QString getSavePosition(){return savePosition_;}

private:
    QQuickWidget *interface_;

    QString url_;
    QString savePosition_;
};

#endif // NEWTASKMSGBOX_H
