import QtQuick 2.14
import Bridge.Constants 1.0
import "../components"
import "../controls"
import QtQuick.Controls 2.12

Rectangle {
    width: height
    height: 300
    anchors.centerIn: parent

    Column {
        anchors.centerIn: parent
        spacing: 16
        BusyIndicator {
            running: true
            anchors.horizontalCenter: parent.horizontalCenter
            width: height
            height: 60
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
            color: Qt.lighter(Constants.colors.primary)
            font.family: Constants.secondary
            text: qsTr("Processing....")
        }
    }
}
