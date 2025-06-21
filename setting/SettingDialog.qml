import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

Rectangle {
    id: root
    width: 480
    height: 580
    color: "#f5f5f5"

    // 公共API函数
    function setDownloadSettings(config) {
        if (config.downloadPath !== undefined) {
            settings.downloadPath = config.downloadPath
            pathInput.text = config.downloadPath
        }
        if (config.soundEnabled !== undefined) {
            settings.soundEnabled = config.soundEnabled
        }
        if (config.notificationEnabled !== undefined) {
            settings.notificationEnabled = config.notificationEnabled
        }
        if (config.maxConcurrent !== undefined) {
            settings.maxConcurrent = config.maxConcurrent
            concurrentInput.text = config.maxConcurrent.toString()
        }
        if (config.speedLimit !== undefined) {
            settings.speedLimit = config.speedLimit
            speedInput.text = config.speedLimit === 0 ? "无限制" : config.speedLimit.toString()
        }
        if (config.autoStart !== undefined) {
            settings.autoStart = config.autoStart
        }
        if (config.detectDuplicate !== undefined) {
            settings.detectDuplicate = config.detectDuplicate
        }
        if (config.deleteTempFiles !== undefined) {
            settings.deleteTempFiles = config.deleteTempFiles
        }
    }

    function getDownloadSettings() {
        return {
            downloadPath: settings.downloadPath,
            soundEnabled: settings.soundEnabled,
            notificationEnabled: settings.notificationEnabled,
            maxConcurrent: settings.maxConcurrent,
            speedLimit: settings.speedLimit,
            autoStart: settings.autoStart,
            detectDuplicate: settings.detectDuplicate,
            deleteTempFiles: settings.deleteTempFiles
        }
    }

    function resetToDefaults() {
        setDownloadSettings({
            downloadPath: "C:/Downloads",
            soundEnabled: true,
            notificationEnabled: true,
            maxConcurrent: 3,
            speedLimit: 0,
            autoStart: false,
            detectDuplicate: true,
            deleteTempFiles: false
        })
    }

    // 单独设置某个选项的便捷函数
    function setDownloadPath(path) {
        settings.downloadPath = path
        pathInput.text = path
    }

    function setSoundEnabled(enabled) {
        settings.soundEnabled = enabled
    }

    function setNotificationEnabled(enabled) {
        settings.notificationEnabled = enabled
    }

    function setMaxConcurrent(count) {
        settings.maxConcurrent = count
        concurrentInput.text = count.toString()
    }

    function setSpeedLimit(limit) {
        settings.speedLimit = limit
        speedInput.text = limit === 0 ? "无限制" : limit.toString()
    }

    function setAutoStart(enabled) {
        settings.autoStart = enabled
    }

    function setDetectDuplicate(enabled) {
        settings.detectDuplicate = enabled
    }

    function setDeleteTempFiles(enabled) {
        settings.deleteTempFiles = enabled
    }

    // 信号，当设置改变时发出
    signal settingsChanged(var newSettings)

    // 监听设置变化
    onSettingsChanged: function(newSettings) {
        console.log("设置已更改:", JSON.stringify(newSettings))
    }

    // 内部状态管理
    QtObject {
        id: settings
        property string downloadPath: "C:/Downloads"
        property bool soundEnabled: true
        property bool notificationEnabled: true
        property int maxConcurrent: 3
        property int speedLimit: 0
        property bool autoStart: false
        property bool detectDuplicate: true
        property bool deleteTempFiles: false
    }

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
                height: 80
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
                                text: settings.downloadPath
                                font.pixelSize: 12
                                color: "#333333"
                                verticalAlignment: TextInput.AlignVCenter
                                selectByMouse: true
                                onTextChanged: settings.downloadPath = text
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
                            checked: settings.soundEnabled
                            onToggled: settings.soundEnabled = checked
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
                            checked: settings.notificationEnabled
                            onToggled: settings.notificationEnabled = checked
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

                            TextInput {
                                id: concurrentInput
                                anchors.centerIn: parent
                                text: settings.maxConcurrent.toString()
                                font.pixelSize: 12
                                color: "#333333"
                                validator: IntValidator { bottom: 1; top: 10 }
                                onTextChanged: {
                                    if (text !== "") {
                                        settings.maxConcurrent = parseInt(text)
                                    }
                                }
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Text {
                            text: "下载速度限制 (KB/s)："
                            font.pixelSize: 12
                            color: "#666666"
                        }

                        Rectangle {
                            width: 120
                            height: 30
                            border.color: "#d0d0d0"
                            border.width: 1
                            radius: 4

                            TextInput {
                                id: speedInput
                                anchors.centerIn: parent
                                text: settings.speedLimit === 0 ? "无限制" : settings.speedLimit.toString()
                                font.pixelSize: 12
                                color: "#333333"
                                validator: IntValidator { bottom: 0; top: 10000 }
                                onTextChanged: {
                                    if (text === "无限制" || text === "") {
                                        settings.speedLimit = 0
                                    } else {
                                        settings.speedLimit = parseInt(text) || 0
                                    }
                                }
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        CustomSwitch {
                            id: autoStartSwitch
                            checked: settings.autoStart
                            onToggled: settings.autoStart = checked
                        }

                        Text {
                            text: "自动开始下载"
                            font.pixelSize: 12
                            color: "#666666"
                        }
                    }
                }
            }

            // 文件管理设置
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
                        text: "文件管理"
                        font.pixelSize: 14
                        font.weight: Font.Medium
                        color: "#333333"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        CustomSwitch {
                            id: duplicateSwitch
                            checked: settings.detectDuplicate
                            onToggled: settings.detectDuplicate = checked
                        }

                        Text {
                            text: "检测重复文件"
                            font.pixelSize: 12
                            color: "#666666"
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        CustomSwitch {
                            id: deleteSwitch
                            checked: settings.deleteTempFiles
                            onToggled: settings.deleteTempFiles = checked
                        }

                        Text {
                            text: "下载完成后删除临时文件"
                            font.pixelSize: 12
                            color: "#666666"
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
                resetToDefaults()
                root.settingsChanged(getDownloadSettings())
            }
        }

        CustomButton {
            text: "取消"
            buttonColor: "#f5f5f5"
            textColor: "#666666"
        }

        CustomButton {
            text: "确定"
            onClicked: {
                var currentSettings = getDownloadSettings()
                console.log("保存设置:")
                console.log("下载路径:", currentSettings.downloadPath)
                console.log("提示音:", currentSettings.soundEnabled)
                console.log("桌面通知:", currentSettings.notificationEnabled)
                console.log("最大下载数:", currentSettings.maxConcurrent)
                console.log("速度限制:", currentSettings.speedLimit)
                console.log("自动开始:", currentSettings.autoStart)
                console.log("检测重复:", currentSettings.detectDuplicate)
                console.log("删除临时文件:", currentSettings.deleteTempFiles)

                root.settingsChanged(currentSettings)
            }
        }
    }

    // 文件夹选择对话框
    FolderDialog {
        id: folderDialog
        title: "选择下载文件夹"
        onAccepted: {
            settings.downloadPath = selectedFolder.toString().replace("file:///", "")
            pathInput.text = settings.downloadPath
        }
    }
}
