import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic

Item
{
    property int currentValue: thrdInput.currentValue;
    ComboBox {
        id: thrdInput
        anchors.fill: parent
        model: ["1", "4", "8", "16"]
        currentIndex: 1

        font.pixelSize: 14

        background: Rectangle {
            anchors.fill: parent;
            color: thrdInput.hovered ? "#f8f9fa" : "#ffffff"
            border.color: thrdInput.activeFocus ? "#007bff" : "#dee2e6"
            border.width: 1
            radius: 6

            Behavior on color { ColorAnimation { duration: 150 } }
            Behavior on border.color { ColorAnimation { duration: 150 } }
        }

        contentItem: Text {
            leftPadding: 12
            rightPadding: 30
            text: thrdInput.displayText
            font: thrdInput.font
            color: "#495057"
            verticalAlignment: Text.AlignVCenter
        }

        indicator: Text {
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: "▼"
            color: "#6c757d"
            font.pixelSize: 10
            rotation: thrdInput.popup.visible ? 180 : 0
            Behavior on rotation { NumberAnimation { duration: 200 } }
        }

        popup: Popup {
            y: thrdInput.height + 2
            width: thrdInput.width
            padding: 4

            background: Rectangle {
                color: "#ffffff"
                border.color: "#dee2e6"
                radius: 6
            }

            contentItem: ListView {
                implicitHeight: contentHeight
                model: thrdInput.delegateModel
                currentIndex: thrdInput.highlightedIndex
            }
        }

        delegate: ItemDelegate {
            width: parent.width
            height: 32

            background: Rectangle {
                color: parent.hovered ? "#e9ecef" : "transparent"
                radius: 4
            }

            contentItem: Text {
                text: modelData
                font: thrdInput.font
                color: "#495057"
                verticalAlignment: Text.AlignVCenter
                leftPadding: 8
            }
        }
    }
}
