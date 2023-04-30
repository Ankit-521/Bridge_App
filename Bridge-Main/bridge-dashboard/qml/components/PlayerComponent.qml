import QtQuick 2.12
import Bridge.Constants 1.0
import QtQuick.Controls 2.5

Rectangle {
    id: mainRectangle
    property alias column: mainColumn
    property alias titleText: nText
    property alias mousearea: mouseArea
    property alias subText: msubText
    width: mainColumn.width
    height: column.height
    border.color: Qt.rgba(12 / 255, 135 / 255, 255 / 255, 0)
    border.width: 1.5
    radius: 4
    Behavior on border.color {
        ColorAnimation {
            duration: 200
        }
    }

    Column {
        id: mainColumn
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 8
        padding: 16
        Rectangle {
            width: height
            height: 100
            color: "#eeeeef"
            radius: width / 2
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                anchors.centerIn: parent
                text: Icons.mdiAccount
                font.family: Constants.iconFont
                color: Qt.rgba(0, 0, 0, 0.05)
                font.pixelSize: 70
            }
            Rectangle {
                width: height
                height: 32
                anchors.bottom: parent.bottom
                x: parent.width - width
                color: Constants.colors.fade
                radius: width / 2
                border.width: 1.5
                border.color: Constants.colors.white
                Text {
                    anchors.centerIn: parent
                    text: Icons.mdiFlag
                    font.family: Constants.iconFont
                    color: Qt.rgba(0, 0, 0, 0.04)
                    font.pixelSize: 20
                }
            }
        }
        Text {
            id: nText
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
            color: Constants.primary
            text: "Player"
            lineHeight: 0.5
            font.family: Constants.secondary
        }
        Text {
            id: msubText
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            color: Constants.colors.primary_light
            text: "0pts"
            font.letterSpacing: 0.5
            font.family: Constants.secondary
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent

        hoverEnabled: true
        onEntered: {
            mainRectangle.border.color = Qt.rgba(12 / 255, 135 / 255,
                                                 255 / 255, 1)
        }
        onExited: {

            mainRectangle.border.color = Qt.rgba(12 / 255, 135 / 255,
                                                 255 / 255, 0)
        }
    }
}
