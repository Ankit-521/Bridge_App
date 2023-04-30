import QtQuick 2.11
import QtQuick.Controls 2.2

import "./controls"
import "./headers"
import "./delegates"

Item {
    visible: (mainWindow.tables == 0)
    width: 300
    height: titleText.implicitHeight
    z: 1
    anchors.centerIn: parent

    property string title: qsTr("No Tournament yet")
    property string description: qsTr("Can`t create a room now.")

    Column {
        width: parent.width
        spacing: 4

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            id: titleText
            width: parent.width
            color: titleTextColor
            font.pixelSize: 40
            font.family: "arial"
            text: title
        }
        Text {
            width: 150
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            // width: parent.width
            color: subTextColor
            font.pixelSize: 16
            font.family: "arial"
            text: description
        }
    }
}
