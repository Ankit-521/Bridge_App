import QtQuick 2.11
import QtQuick.Controls 2.2

import "../controls"

ItemDelegate {
    width: parent.width
    height: 56
    Column{
        width: parent.width
        spacing: 2
        anchors.centerIn: parent
        padding: 16
        Text {
            width: parent.width
            color: titleTextColor
            font.pixelSize: 16
            font.family: "arial"
            text: qsTr(index==0?"You":"user-@3343443")
        }
        Text {
            width: parent.width
            font.family: "arial"
            font.pixelSize: 14
            color: subTextColor
            text: qsTr("east")
        }
    }

}
