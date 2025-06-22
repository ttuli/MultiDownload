import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt5Compat.GraphicalEffects
import QtQuick.Dialogs 6.5
import Qt.labs.platform
import QtQuick.LocalStorage

Rectangle {
    id: root
    color: "#f8f9fa"
    radius: 12
    border.color: "#e9ecef"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // 标题栏
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "新建下载任务"
                font.pixelSize: 18
                font.weight: Font.DemiBold
                color: "#212529"
            }

            Item { Layout.fillWidth: true }

            // 关闭按钮
            Rectangle {
                width: 32
                height: 32
                radius: 16
                color: "transparent"

                Text {
                    anchors.centerIn: parent
                    text: "✕"
                    font.pixelSize: 18
                    color: "#6c757d"
                    font.bold: true;
                }

                MouseArea {
                    id: closeMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        rootWidget.reject();
                    }
                    onEntered: {
                        parent.color="lightgrey";
                    }
                    onExited: {
                        parent.color="transparent";
                    }
                }
            }
        }

        // 输入区域
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 16

            // URL输入
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "下载链接"
                    font.pixelSize: 14
                    font.weight: Font.Medium
                    color: "#495057"
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 44
                    color: "#ffffff"
                    border.color: urlInput.activeFocus ? "#007bff" : "#dee2e6"
                    border.width: urlInput.activeFocus ? 2 : 1
                    radius: 8

                    TextInput {
                        id: urlInput
                        anchors.fill: parent
                        anchors.margins: 12
                        font.pixelSize: 14
                        color: "#212529"
                        selectByMouse: true
                        clip: true;

                        Text {
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: "请输入下载链接..."
                            color: "#6c757d"
                            font.pixelSize: 14
                            visible: urlInput.text.length === 0 && !urlInput.activeFocus
                        }
                    }
                }
            }


            // 保存路径
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "保存位置"
                    font.pixelSize: 14
                    font.weight: Font.Medium
                    color: "#495057"
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Rectangle {
                        Layout.fillWidth: true
                        height: 44
                        color: "#ffffff"
                        border.color: pathInput.activeFocus ? "#007bff" : "#dee2e6"
                        border.width: pathInput.activeFocus ? 2 : 1
                        radius: 8

                        TextInput {
                            id: pathInput
                            anchors.fill: parent
                            anchors.margins: 12
                            font.pixelSize: 14
                            color: "#212529"
                            selectByMouse: true
                            readOnly: true;
                            text: {
                                var path=new URL(StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]).pathname;
                                return path.substring(1);
                            }
                            clip:true
                        }
                    }

                    // 浏览按钮
                    Rectangle {
                        width: 80
                        height: 44
                        color: browseMouseArea.containsMouse ? "#e9ecef" : "#f8f9fa"
                        border.color: "#dee2e6"
                        border.width: 1
                        radius: 8

                        Text {
                            anchors.centerIn: parent
                            text: "浏览"
                            font.pixelSize: 14
                            color: "#495057"
                        }

                        FolderDialog{
                            id:folderDialog;
                            title: "选择保存位置";
                            options: FolderDialog.DontResolveSymlinks;
                            // currentFolder: StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]
                            onAccepted: {
                                var path = new URL(folderDialog.folder)
                                pathInput.text=path.pathname.substring(1)
                            }
                        }

                        MouseArea {
                            id: browseMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                folderDialog.open();
                            }
                        }
                    }
                }
            }

            //线程数
            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "线程数"
                    font.pixelSize: 14
                    font.weight: Font.Medium
                    color: "#495057"
                }

                Rectangle {
                    Layout.preferredWidth: 100;
                    height: 44
                    color:"transparent"
                    radius: 8


                    CusComboBox{
                        id:thrdInput
                        anchors.fill: parent;
                        anchors.topMargin: 5;
                        anchors.bottomMargin: 5;
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }

        // 按钮区域
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Item { Layout.fillWidth: true }

            // 取消按钮
            Rectangle {
                width: 80
                height: 36
                color: cancelMouseArea.containsMouse ? "#f8f9fa" : "transparent"
                border.color: "#dee2e6"
                border.width: 1
                radius: 6

                Text {
                    anchors.centerIn: parent
                    text: "取消"
                    font.pixelSize: 14
                    color: "#6c757d"
                }

                MouseArea {
                    id: cancelMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        rootWidget.reject();
                    }
                }
            }

            // 确认按钮
            Rectangle {
                width: 80
                height: 36
                color: {
                    if (!confirmMouseArea.enabled) return "#adb5bd"
                    return confirmMouseArea.containsMouse ? "#0056b3" : "#007bff"
                }
                radius: 6

                property bool enabled: urlInput.text.trim().length > 0

                Text {
                    anchors.centerIn: parent
                    text: "开始"
                    font.pixelSize: 14
                    font.weight: Font.Medium
                    color: "#ffffff"
                }

                MouseArea {
                    id: confirmMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    enabled: parent.enabled
                    onClicked: {
                        rootWidget.saveDownloadInfo(urlInput.text,pathInput.text,thrdInput.currentValue)
                        rootWidget.accept();
                    }
                }
            }
        }
    }
}
