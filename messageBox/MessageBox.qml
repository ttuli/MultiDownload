import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Rectangle {
    id: root
    color: "#f0f0f0"

    // 属性定义
    property string title: "提示"
    property string message: "这是一条消息提示"
    property string messageType: "info" // info, warning, error, success
    property bool showErrorButton: true

    // 主对话框容器
    Rectangle {
        id: dialogContainer
        anchors.fill: parent;
        color: "#ffffff"

        // 弹出动画
        scale: root.visible ? 1.0 : 0.8
        opacity: root.visible ? 1.0 : 0

        Behavior on scale {
            NumberAnimation {
                duration: 250
                easing.type: Easing.OutCubic
            }
        }

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }

        // 标题栏
        Rectangle {
            id: titleBar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 60
            color: getTitleBarColor()

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 16
                color: parent.color
            }

            // 图标
            Rectangle {
                id: iconContainer
                width: 32
                height: 32
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                color: "#ffffff"
                radius: 16

                Text {
                    anchors.centerIn: parent
                    text: getIconText()
                    font.family: "Segoe UI Emoji"
                    font.pixelSize: 18
                    color: getTitleBarColor()
                }
            }

            // 标题文本
            Text {
                anchors.left: iconContainer.right
                anchors.leftMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                text: root.title
                font.family: "Microsoft YaHei"
                font.pixelSize: 16
                font.weight: Font.Medium
                color: "#ffffff"
            }

            // 错误按钮
            Rectangle {
                id: errorButton
                visible: root.showErrorButton
                width: 28
                height: 28
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                color:"transparent"
                radius: 14
                border.color: "#ffffff"
                border.width: 1
                opacity: errorButtonArea.containsMouse ? 1.0 : 0.7

                Text {
                    anchors.centerIn: parent
                    text: "×"
                    font.family: "Microsoft YaHei"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    color: errorButtonArea.containsMouse ? getTitleBarColor() : "#ffffff"
                }

                MouseArea {
                    id: errorButtonArea
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true;
                    onClicked: {
                        rootWidget.onReject();
                    }
                    onEntered: {
                        parent.color="#ffffff";
                    }
                    onExited: {
                        parent.color="transparent";
                    }
                }

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }
        }

        // 消息内容区域
        Rectangle {
            id: contentArea
            anchors.top: titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: buttonArea.top
            color: "transparent"

            ScrollView {
                anchors.fill: parent
                anchors.margins: 20

                Text {
                    width: contentArea.width - 40
                    text: root.message
                    font.family: "Microsoft YaHei"
                    font.pixelSize: 14
                    color: "#333333"
                    wrapMode: Text.WordWrap
                    lineHeight: 1.4
                }
            }
        }

        // 按钮区域
        Rectangle {
            id: buttonArea
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 70
            color: "#f8f9fa"

            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 16
                color: parent.color
            }

            Row {
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                spacing: 12

                // 取消按钮
                Rectangle {
                    id: cancelButton
                    width: 80
                    height: 36
                    color: cancelButtonArea.containsMouse ? "#e9ecef" : "transparent"
                    border.color: "#dee2e6"
                    border.width: 1
                    radius: 8

                    Text {
                        anchors.centerIn: parent
                        text: "取消"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 14
                        color: "#6c757d"
                    }

                    MouseArea {
                        id: cancelButtonArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            rootWidget.onReject()
                        }
                    }

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }

                // 确定按钮
                Rectangle {
                    id: confirmButton
                    width: 80
                    height: 36
                    color: confirmButtonArea.containsMouse ? getConfirmHoverColor() : getConfirmColor()
                    radius: 8

                    Text {
                        anchors.centerIn: parent
                        text: "确定"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 14
                        font.weight: Font.Medium
                        color: "#ffffff"
                    }

                    MouseArea {
                        id: confirmButtonArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            rootWidget.onAccept()
                        }
                    }

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }
            }
        }
    }

    // 辅助函数
    function getTitleBarColor() {
        switch(messageType) {
            case "error": return "#dc3545"
            case "warning": return "#fd7e14"
            case "success": return "#198754"
            default: return "#0d6efd"
        }
    }

    function getIconText() {
        switch(messageType) {
            case "error": return "❌"
            case "warning": return "⚠️"
            case "success": return "✅"
            default: return "ℹ️"
        }
    }

    function getConfirmColor() {
        switch(messageType) {
            case "error": return "#dc3545"
            case "warning": return "#fd7e14"
            case "success": return "#198754"
            default: return "#0d6efd"
        }
    }

    function getConfirmHoverColor() {
        switch(messageType) {
            case "error": return "#bb2d3b"
            case "warning": return "#e8681c"
            case "success": return "#146c43"
            default: return "#0b5ed7"
        }
    }

    function setTitleAContent(mtitle,mcontent,msgType){
        title=mtitle;
        message=mcontent;
        messageType=msgType;
    }
}
