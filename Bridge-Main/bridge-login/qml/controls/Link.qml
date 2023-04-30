import QtQuick 2.14
import Bridge.Constants 1.0
import QtQuick.Controls 2.12
import "../controls"

Text {
    property url link:""
    font.letterSpacing: 0.4
   MouseArea {
        hoverEnabled: true
        onEntered: {
            parent.color = Qt.rgba(0, 0, 0, 0.7)
        }
        onExited: {
            parent.color = Qt.rgba(0, 0, 0, 0.4)
        }
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            Qt.openUrlExternally(link)
        }
    }
}
