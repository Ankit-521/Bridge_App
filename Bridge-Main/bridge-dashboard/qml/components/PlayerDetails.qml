import QtQuick 2.14
import Bridge.Constants 1.0
import QtQuick.Controls 2.5
import "../controls"

Popup {
    clip: true
    id: control
    width: 400
    height: mainItem.height
    x: parent.width - width
    modal: true
    padding: 0
    background: Rectangle {}
    enter: Transition {
        ParallelAnimation {
            NumberAnimation {
                property: "opacity"
                from: 0.0
                to: 1.0
                duration: 200
            }
            NumberAnimation {
                property: "x"
                from: control.parent.width + 50
                to: control.parent.width - (control.width)
                duration: 300
                easing.type: Easing.OutExpo
            }
        }
    }
    exit: Transition {
        ParallelAnimation {
            NumberAnimation {
                property: "opacity"
                from: 0.0
                to: 1.0
                duration: 200
            }
            NumberAnimation {
                property: "x"
                from: control.x
                to: control.parent.width + 50
                duration: 400
                easing.type: Easing.OutExpo
            }
        }
    }

    ScrollView {
        anchors.fill: parent
        Column {
            width: parent.width
            spacing: 8
            IconButton {
                z: 1
                width: parent.width
                height: 200
                color: Constants.colors.placeHolderColor
                radius: 0
                textItem.text: Icons.mdiCards
                enabled: false
                textItem.font.pixelSize: 80

                IconButton {
                    width: height
                    height: 100
                    color: Constants.colors.placeHolderColor
                    border.color: Constants.colors.white
                    border.width: 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: parent.height - (height / 2)

                    textItem.text: Icons.mdiAccount
                    enabled: false
                    textItem.font.pixelSize: 60
                }
            }
            Repeater {
                model: 5
                delegate: Rectangle {
                    width: parent.width
                    height: 400
                    color: "pink"
                }
            }
        }
    }
}
