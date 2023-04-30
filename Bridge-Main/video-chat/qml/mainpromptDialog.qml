import QtQuick 2.11
import QtQuick.Controls 2.2

Item {
    height: mainRow.height
    Component.onCompleted: {
        width = parentDialog.width
    }

    Row {
        id: mainRow
        width: parent.width
        padding: 16
        spacing: 16
        Rectangle {
            id: dialogIcon
            width: height
            height: 56
            color: "#f5f5f5"
            radius: width / 2
        }
        Rectangle {

            width: parent.width - (dialogIcon.width + mainRow.padding + mainRow.spacing)

            height: mainCol.height
            Column {
                id: mainCol
                width: parent.width
                spacing: 8
                Text {
                    width: parent.width
                    wrapMode: Text.WordWrap
                    font.pixelSize: 24
                    font.family: "arial"
                    color: "#212121"
                    text: parentDialog.title
                }
                Text {
                    width: parent.width
                    wrapMode: Text.WordWrap
                    font.pixelSize: 14
                    font.family: "arial"
                    color: "#616161"
                    text: parentDialog.details
                }
            }
        }
    }
}
