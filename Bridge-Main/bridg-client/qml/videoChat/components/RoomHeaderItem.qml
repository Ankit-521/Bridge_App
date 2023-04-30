import QtQuick 2.11
import QtQuick.Controls 2.12
import "../controls"

Item {
    width: parent.width
    height: headerColumn.height

    Column {
        id: headerColumn
        width: parent.width
        spacing: 4
        topPadding: bottomPadding
        bottomPadding: 16
        Text {
            font.pixelSize: 50
            font.family: "Arial"
            font.weight: Font.Light
            text: qsTr("Players")
            font.letterSpacing: 2
            color: Qt.rgba(1, 1, 1, 1)
        }
    }
    Divider {
        width: parent.width - (mainCol.leftPadding * 2)
        y: parent.height - height
    }
}
