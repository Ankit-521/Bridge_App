import QtQuick 2.14
import Bridge.Constants 1.0
import QtQuick.Controls 2.12
import "../controls"

Item {
    width: parent.width
    height: 36

    Divider {}

    // left row item
    Row {
        anchors.verticalCenter: parent.verticalCenter
        spacing: 16
        leftPadding: 16
        Repeater {
            model: ListModel {
                Component.onCompleted: {
                    append({
                               "text": "Privacy policy"
                           })
                    append({
                               "text": "dassbridge.com"
                           })
                }
            }
            delegate: Link {
                color: Qt.rgba(0, 0, 0, 0.4) //Constants.colors.secondary
                text: qsTr(model.text)
                font.family: Constants.secondary
                font.letterSpacing: 3
                font.pixelSize: 10
                link: "https://google.com"
                font.capitalization: Font.AllUppercase
            }
        }
    }

    Row {
        anchors.verticalCenter: parent.verticalCenter
        spacing: 16
        leftPadding: 16
        anchors.right: parent.right
        anchors.rightMargin: 16

        // Flags
        Row {
            spacing: 16
            Repeater {
                model: 2
                delegate: Rectangle {
                    width: 48
                    height: 28
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        fillMode: Image.PreserveAspectFit
                        anchors.fill: parent
                        anchors.centerIn: parent
                        smooth: true
                        mipmap: true
                        source: "../../common_assets/images/logo.jpg"
                    }
                }
            }
        }
    }
}
