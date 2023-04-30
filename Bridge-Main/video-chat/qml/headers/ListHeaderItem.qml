import QtQuick 2.11
import QtQuick.Controls 2.2

import "../controls"
Item{
    width: parent.width
    height: 100
    Column{
        width: parent.width
        spacing: 2
        anchors.centerIn: parent
        padding: 16
        Text {
            width: parent.width
            color: titleTextColor
            font.pixelSize: 32
            font.family: "arial"
            text: qsTr("Players")
        }
        Text {
            width: parent.width
            font.family: "arial"
            font.pixelSize: 14
            color: subTextColor
            text: qsTr("5 connects")
        }
    }
    
    Divider {
        y:parent.height-height
    }
}
