import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs
import Qt.labs.platform
import QtQuick.LocalStorage
import "../messageBox"

Rectangle {
    id: root
    color: "#f5f5f5";

    // 自定义开关组件
    component CustomSwitch: Rectangle {
        id: switchRoot
        property bool checked: false
        signal toggled()

        width: 44
        height: 24
        radius: 12
        color: checked ? "#2196F3" : "#cccccc"

        Rectangle {
            id: handle
            width: 20
            height: 20
            radius: 10
            x: switchRoot.checked ? parent.width - width - 2 : 2
            y: 2
            color: "white"

            Behavior on x {
                NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                switchRoot.checked = !switchRoot.checked
                switchRoot.toggled()
            }
        }
    }

    // 自定义按钮组件
    component CustomButton: Rectangle {
        id: buttonRoot
        property string text: ""
        property color buttonColor: "#2196F3"
        property color textColor: "white"
        signal clicked()

        width: buttonText.implicitWidth + 20
        height: 35
        radius: 4
        color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.2) :
               (buttonMouseArea.containsMouse ? Qt.lighter(buttonColor, 1.1) : buttonColor)

        Text {
            id: buttonText
            anchors.centerIn: parent
            text: buttonRoot.text
            font.pixelSize: 12
            color: buttonRoot.textColor
        }

        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: buttonRoot.clicked()
        }
    }

    QtObject{
        id:tmpConfig
        property string downloadPath
        property bool soundEnabled
        property bool notificationEnabled
        property int maxConcurrent
        property int downloadThrd
        onDownloadPathChanged: {
            rootWidget.changeNotSave=true
        }
        onSoundEnabledChanged: {
            rootWidget.changeNotSave=true
        }
        onNotificationEnabledChanged: {
            rootWidget.changeNotSave=true
        }
        onDownloadThrdChanged: {
            rootWidget.changeNotSave=true
        }
        onMaxConcurrentChanged: {
            rootWidget.changeNotSave=true
        }
    }

    Component.onCompleted: {
        init()
    }

    function init(){
        tmpConfig.downloadPath=config.downloadPath;
        tmpConfig.soundEnabled=config.soundEnabled;
        tmpConfig.notificationEnabled=config.notificationEnabled;
        tmpConfig.maxConcurrent=config.maxConcurrent;
        tmpConfig.downloadThrd=config.downloadThrd;
        rootWidget.setChangeNotSave(false)
    }

    // 标题栏
    Rectangle {
        id: titleBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 60
        color: "#2196F3"

        Text {
            anchors.centerIn: parent
            text: "下载器设置"
            font.pixelSize: 18
            font.weight: Font.Medium
            color: "white"
        }
    }

    // 主内容区域
    ScrollView {
        id: scrollView
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttonRow.top
        anchors.margins: 20

        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            width: scrollView.width - 40
            spacing: 25

            // 下载路径设置
            Rectangle {
                Layout.fillWidth: true
                height: 95
                color: "white"
                radius: 8
                border.color: "#e0e0e0"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 8

                    Text {
                        text: "默认保存路径"
                        font.pixelSize: 14
                        font.weight: Font.Medium
                        color: "#333333"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Rectangle {
                            Layout.fillWidth: true
                            height: 35
                            color: "#f8f8f8"
                            border.color: "#d0d0d0"
                            border.width: 1
                            radius: 4

                            TextInput {
                                id: pathInput
                                anchors.fill: parent
                                anchors.margins: 8
                                clip: true
                                readOnly: true
                                text: config.downloadPath
                                font.pixelSize: 12
                                color: "#333333"
                                verticalAlignment: TextInput.AlignVCenter
                                selectByMouse: true
                            }
                        }

                        CustomButton {
                            text: "浏览"
                            onClicked: folderDialog.open()
                        }
                    }
                }
            }

            // 通知设置
            Rectangle {
                Layout.fillWidth: true
                height: 120
                color: "white"
                radius: 8
                border.color: "#e0e0e0"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 12

                    Text {
                        text: "通知设置"
                        font.pixelSize: 14
                        font.weight: Font.Medium
                        color: "#333333"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        CustomSwitch {
                            id: soundSwitch
                            checked: tmpConfig.soundEnabled
                            onToggled: tmpConfig.soundEnabled = checked
                        }

                        Text {
                            text: "下载完成提示音"
                            font.pixelSize: 12
                            color: "#666666"
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        CustomSwitch {
                            id: notificationSwitch
                            checked: tmpConfig.notificationEnabled
                            onToggled: tmpConfig.notificationEnabled = checked
                        }

                        Text {
                            text: "桌面通知"
                            font.pixelSize: 12
                            color: "#666666"
                        }
                    }
                }
            }

            // 下载设置
            Rectangle {
                Layout.fillWidth: true
                height: 160
                color: "white"
                radius: 8
                border.color: "#e0e0e0"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 12

                    Text {
                        text: "下载设置"
                        font.pixelSize: 14
                        font.weight: Font.Medium
                        color: "#333333"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Text {
                            text: "默认下载线程数"
                            font.pixelSize: 12
                            color: "#666666"
                        }

                        Rectangle {
                            width: 100
                            height: 30
                            border.color: "#d0d0d0"
                            border.width: 1
                            radius: 4
                            color: "transparent"

                            CusComboBox{
                                id:downloadThrdBox
                                anchors.centerIn: parent
                                width: 100
                                height: 35
                                currentIndex_: {
                                    if(tmpConfig.downloadThrd===1)
                                        return 0
                                    if(tmpConfig.downloadThrd===4)
                                        return 1
                                    if(tmpConfig.downloadThrd===8)
                                        return 2
                                    if(tmpConfig.downloadThrd===16)
                                        return 3
                                }
                                onCurrentValueChanged: {
                                    tmpConfig.downloadThrd=currentValue;
                                }
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Text {
                            text: "最大同时下载数："
                            font.pixelSize: 12
                            color: "#666666"
                        }

                        Rectangle {
                            width: 100
                            height: 30
                            border.color: "#d0d0d0"
                            border.width: 1
                            radius: 4
                            color: "transparent"

                            CusComboBox{
                                id:maxConcurentBox
                                anchors.centerIn: parent
                                width: 100
                                height: 35
                                model_: ["1","2"]
                                currentIndex_: {
                                    if(tmpConfig.maxConcurrent===2)
                                        return 1
                                    if(tmpConfig.maxConcurrent===1)
                                        return 0
                                }

                                onCurrentValueChanged: {
                                    tmpConfig.maxConcurrent=currentValue;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // 底部按钮
    RowLayout {
        id: buttonRow
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        height: 40
        spacing: 15

        Item {
            Layout.fillWidth: true
        }

        CustomButton {
            text: "重置"
            buttonColor: "#f5f5f5"
            textColor: "#666666"
            onClicked: {
                tmpConfig.downloadPath=new URL(StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]).pathname.substring(1);
                tmpConfig.downloadThrd=4;
                tmpConfig.notificationEnabled=true
                tmpConfig.soundEnabled=true
                tmpConfig.maxConcurrent=2
            }
        }

        CustomButton {
            text: "取消"
            buttonColor: "#f5f5f5"
            textColor: "#666666"
            onClicked: {
                rootWidget.setChangeNotSave(false)
                rootWidget.close()
            }
        }

        CustomButton {
            text: "确定"
            onClicked: {
                saveSetting()
                rootWidget.setChangeNotSave(false)
                rootWidget.close()
            }
        }
    }

    function saveSetting(){
        config.setDownloadPath(tmpConfig.downloadPath)
        config.setDownloadThrd(tmpConfig.downloadThrd)
        config.setMaxConcurrent(tmpConfig.maxConcurrent)
        config.setNotificationEnabled(tmpConfig.notificationEnabled)
        config.setSoundEnabled(tmpConfig.soundEnabled)
    }

    // 文件夹选择对话框
    FolderDialog {
        id: folderDialog
        title: "选择下载文件夹"
        onAccepted: {
            tmpConfig.downloadPath = folder.toString().replace("file:///", "")
            pathInput.text = tmpConfig.downloadPath
        }
    }
}
