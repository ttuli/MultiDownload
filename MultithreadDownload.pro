QT       += core gui network quickwidgets concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base/framless_dialog.cpp \
    main.cpp \
    messageBox/custommessagebox.cpp \
    messageBox/newtaskmsgbox.cpp \
    model/taskmodel.cpp \
    setting/settingdialog.cpp \
    singledownloadtask.cpp \
    widget.cpp

HEADERS += \
    base/framless_dialog.h \
    messageBox/custommessagebox.h \
    messageBox/newtaskmsgbox.h \
    model/taskmodel.h \
    setting/settingdialog.h \
    singledownloadtask.h \
    widget.h

LIBS+= -ldwmapi

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc
