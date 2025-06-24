import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "messageBox"

Rectangle {
    id: window

    property color backgroundColor: "#1e1e2e"
    property color surfaceColor: "#313244"
    property color primaryColor: "#89b4fa"
    property color secondaryColor: "#a6adc8"
    property color accentColor: "#f38ba8"
    property color successColor: "#a6e3a1"
    property color warningColor: "#f9e2af"
    property color errorColor: "#f38ba8"
    property color cancelledColor: "#7f849c"
    property color textColor: "#cdd6f4"
    property color mutedTextColor: "#6c7086"

    color: backgroundColor

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: surfaceColor
            radius: 12

            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                Button {
                    id: addTaskBtn
                    Layout.preferredWidth: 90
                    Layout.fillHeight: true

                    background: Rectangle {
                        color: addTaskBtn.hovered ? Qt.darker(primaryColor, 1.1) : primaryColor
                        radius: 8

                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    contentItem: RowLayout {
                        anchors.fill: parent;
                        anchors.topMargin: 3;
                        anchors.leftMargin: 5;
                        spacing: 5

                        Text {
                            text: "+"
                            color: "white"
                            font.pixelSize: 18
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter;
                            verticalAlignment: Text.AlignVCenter;
                        }

                        Text {
                            text: "添加任务"
                            color: "white"
                            font.pixelSize: 14
                            font.bold: true;
                            horizontalAlignment: Text.AlignHCenter;
                            verticalAlignment: Text.AlignVCenter;
                        }
                    }

                    onClicked: {
                        //TODO 添加下载任务
                        rootWidget.addTask();
                    }
                }

                Button {
                    id: setiingBtn;
                    Layout.preferredWidth: 70
                    Layout.fillHeight: true

                    background: Rectangle {
                        color: setiingBtn.hovered ? Qt.darker("lightgrey", 1.1) : "lightgrey";
                        radius: 8

                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    contentItem: RowLayout {
                        anchors.fill: parent;
                        anchors.topMargin: 3;
                        spacing: 5

                        Text {
                            text: "⚙"
                            color: "white"
                            font.pixelSize: 18
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter;
                            verticalAlignment: Text.AlignVCenter;
                        }

                        Text {
                            text: "设置"
                            color: "black"
                            font.pixelSize: 14
                            font.bold: true;
                            horizontalAlignment: Text.AlignHCenter;
                            verticalAlignment: Text.AlignVCenter;
                        }
                    }

                    onClicked: {
                        //TODO 进行设置
                        rootWidget.doSetting();
                    }
                }

                Item { Layout.fillWidth: true }

                // 全部开始按钮
                Button {
                    id: startAllBtn
                    Layout.preferredWidth: 100
                    Layout.fillHeight: true

                    background: Rectangle {
                        color: startAllBtn.hovered ? Qt.darker(successColor, 1.1) : successColor
                        radius: 8

                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    contentItem: Text {
                        text: "全部开始"
                        color: backgroundColor
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        rootWidget.startAllTask()
                    }
                }

                //全部暂停
                Button {
                    id: pauseAllBtn
                    Layout.preferredWidth: 100
                    Layout.fillHeight: true

                    background: Rectangle {
                        color: pauseAllBtn.hovered ? Qt.darker(warningColor, 1.1) : warningColor
                        radius: 8

                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    contentItem: Text {
                        text: "全部暂停"
                        color: backgroundColor
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        rootWidget.pauseAllTask()
                    }
                }

                // 全部取消按钮
                Button {
                    id: cancelAllBtn
                    Layout.preferredWidth: 100
                    Layout.fillHeight: true

                    background: Rectangle {
                        color: cancelAllBtn.hovered ? Qt.darker(errorColor, 1.1) : errorColor
                        radius: 8

                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    contentItem: Text {
                        text: "全部取消"
                        color: "white"
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        rootWidget.cancelAllTask()
                    }
                }
            }
        }

        // 主要内容区域
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: surfaceColor
            radius: 12

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 0
                spacing: 0

                // 列表头部
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    color: Qt.darker(surfaceColor, 1.1)
                    radius: 12

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 20
                        anchors.rightMargin: 20
                        spacing: 15

                        Text {
                            id: filename_text
                            text: "文件名"
                            color: secondaryColor
                            font.pixelSize: 14
                            font.bold: true
                            Layout.preferredWidth: 250;
                            Layout.fillWidth: true
                        }

                        Text {
                            id: progress_text
                            text: "进度"
                            color: secondaryColor
                            font.pixelSize: 14
                            font.bold: true
                            Layout.preferredWidth: 150;
                            Layout.fillWidth: true
                        }

                        Text {
                            id: speed_text
                            text: "速度"
                            color: secondaryColor
                            font.pixelSize: 14
                            font.bold: true
                            Layout.preferredWidth: 80
                        }

                        Text {
                            id: size_text
                            text: "大小"
                            color: secondaryColor
                            font.pixelSize: 14
                            font.bold: true
                            Layout.preferredWidth: 80
                        }

                        Text {
                            id: status_text
                            text: "状态"
                            color: secondaryColor
                            font.pixelSize: 14
                            font.bold: true
                            Layout.preferredWidth: 40
                        }
                    }
                }

                // 下载列表
                ListView {
                    id: downloadList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: taskModel
                    clip: true

                    property int selectedIndex: -1

                    delegate: Rectangle {
                        width: downloadList.width
                        height: 80
                        color: index === downloadList.selectedIndex ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : "transparent"

                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            height: 1
                            color: Qt.rgba(secondaryColor.r, secondaryColor.g, secondaryColor.b, 0.2)
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                downloadList.selectedIndex = index
                            }
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20
                            anchors.rightMargin: 20
                            spacing: 15

                            // 文件名
                            ColumnLayout {
                                Layout.preferredWidth: 250
                                spacing: 5

                                Text {
                                    Layout.fillWidth: true
                                    text: model_filename
                                    color: textColor
                                    font.pixelSize: 14
                                    font.bold: true
                                    elide: Text.ElideRight
                                }

                                Text {
                                    Layout.fillWidth: true
                                    text: model_url
                                    color: mutedTextColor
                                    font.pixelSize: 11
                                    elide: Text.ElideRight
                                }
                            }

                            // 进度条
                            ColumnLayout {
                                Layout.preferredWidth: 150
                                spacing: 5

                                ProgressBar {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 8
                                    value: model_progress/100;
                                    visible: {
                                        if(model_status==="cancel")
                                            return false
                                        return true
                                    }

                                    background: Rectangle {
                                        color: {
                                            if(model_status!=="completed")
                                                return Qt.rgba(mutedTextColor.r, mutedTextColor.g, mutedTextColor.b, 0.3)
                                            return successColor
                                        }
                                        radius: 4
                                    }

                                    contentItem: Item {
                                        Rectangle {
                                            width: parent.width * parent.parent.value
                                            height: parent.height
                                            radius: 4
                                            color: getColorByStatus(model_status)
                                        }
                                    }
                                }

                                Text {
                                    text: {
                                        return model_progress + "%"
                                    }
                                    visible: {
                                        if(model_status==="cancel")
                                            return false
                                        return true
                                    }

                                    color: textColor
                                    font.pixelSize: 12
                                }
                            }

                            // 速度
                            Text {
                                Layout.preferredWidth: 90
                                text: {
                                    if(model_status!=="completed"&&model_status!=="paused"&&model_status!=="cancel")
                                        return model_speed
                                    return "";
                                }

                                color: textColor
                                font.pixelSize: 12
                            }

                            // 大小
                            Text {
                                Layout.preferredWidth: 75
                                text: model_size
                                color: textColor
                                font.pixelSize: 12
                            }

                            // 状态
                            Text {
                                Layout.preferredWidth: 45
                                text: getTextByStatus(model_status)
                                color: getColorByStatus(model_status)
                                font.pixelSize: 12
                            }
                        }
                    }
                }
            }
        }

        // 底部操作栏
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: surfaceColor
            radius: 12

            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                Text {
                    id:selectFileName;
                    text: downloadList.selectedIndex >= 0 ? "已选中: " + taskModel.GetName(downloadList.selectedIndex) : "请选择一个任务"
                    color: downloadList.selectedIndex >= 0 ? textColor : mutedTextColor
                    font.pixelSize: 12
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }

                // 单个任务操作按钮
                Button {
                    id: startBtn
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: 35
                    enabled: downloadList.selectedIndex >= 0

                    background: Rectangle {
                        color: startBtn.enabled ? (startBtn.hovered ? Qt.darker(successColor, 1.1) : successColor) : Qt.rgba(successColor.r, successColor.g, successColor.b, 0.3)
                        radius: 6

                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    contentItem: Text {
                        text: "开始"
                        color: startBtn.enabled ? backgroundColor : mutedTextColor
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        if (downloadList.selectedIndex >= 0) {
                            // taskModel.setProperty(downloadList.selectedIndex, "status", "downloading")
                            rootWidget.startTask(taskModel.GetId(downloadList.selectedIndex),downloadList.selectedIndex)
                        }
                    }
                }

                Button {
                    id: pauseBtn
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: 35
                    enabled: downloadList.selectedIndex >= 0

                    background: Rectangle {
                        color: pauseBtn.enabled ? (pauseBtn.hovered ? Qt.darker(warningColor, 1.1) : warningColor) : Qt.rgba(warningColor.r, warningColor.g, warningColor.b, 0.3)
                        radius: 6

                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    contentItem: Text {
                        text: "暂停"
                        color: pauseBtn.enabled ? backgroundColor : mutedTextColor
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        if (downloadList.selectedIndex >= 0) {
                            rootWidget.pauseTask(taskModel.GetId(downloadList.selectedIndex),downloadList.selectedIndex)
                        }
                    }
                }

                Button {
                    id: cancelBtn
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: 35
                    enabled: downloadList.selectedIndex >= 0

                    background: Rectangle {
                        color: cancelBtn.enabled ? (cancelBtn.hovered ? Qt.darker(errorColor, 1.1) : errorColor) : Qt.rgba(errorColor.r, errorColor.g, errorColor.b, 0.3)
                        radius: 6

                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    contentItem: Text {
                        text: "取消"
                        color: "white"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        if (downloadList.selectedIndex >= 0) {
                            // taskModel.setProperty(downloadList.selectedIndex, "status", "paused")
                            // taskModel.setProperty(downloadList.selectedIndex, "speed", "0 B/s")
                            rootWidget.cancelTask(taskModel.GetId(downloadList.selectedIndex),downloadList.selectedIndex)
                        }
                    }
                }
            }
        }
    }

    function getColorByStatus(val){
        switch(val) {
            case "completed": return successColor
            case "downloading": return primaryColor
            case "paused": return warningColor
            case "error": return errorColor
            case "cancel": return cancelledColor
            case "waitting": return mutedTextColor
            case "merging": return "white"
            default: return "black"
        }
    }
    function getTextByStatus(val){
        switch(val) {
            case "completed": return "已完成"
            case "downloading": return "下载中"
            case "paused": return "暂停"
            case "error": return "下载失败"
            case "cancel": return "已取消"
            case "waitting": return "等待中"
            case "merging": return "合并中"
            default: return ""
        }
    }

    function showMsg(msg,type,duration){
        msgPopup.showMessage(msg,type,duration);
    }

    MessagePopup{
        id:msgPopup
        anchors.fill: parent;
    }
}
