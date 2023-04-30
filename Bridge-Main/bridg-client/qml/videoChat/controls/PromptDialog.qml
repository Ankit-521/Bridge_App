import QtQuick 2.14
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
Popup {
    id: popUp
    readonly property real key: innerObject.requestKey

    signal clicked(var action)

    width: 400
    height: 160
    padding: 24

    closePolicy: Popup.NoAutoClose
    modal: true
    background: Rectangle {
        id: bg
        radius: 8
    }
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2
    Column {
        id: contentColumn
        width: parent.width
        spacing: 8
        Text {
            id: titleText
            font.pixelSize: 28
            font.family: "Arial"
            font.letterSpacing: 0.3
            color: Qt.rgba(0, 0, 0, 1)
        }
        Text {
            id: descriptionText
            width: parent.width
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pixelSize: 16
            font.family: "Arial"
            font.weight: Font.Light
            color: Qt.rgba(0, 0, 0, 0.7)
        }
        Rectangle {
            width: parent.width
            height: 48

            Row {
                topPadding: 4
                anchors.right: parent.right
                anchors.rightMargin: 0
                spacing: 8
                Repeater {
                    model: 2
                    delegate: Button {
                        id: btn
                        text: index == 0 ? "Yes" : "No"
                        height: 36
                        width: 70
                        font.pixelSize: 14
                        font.family: "arial"
                        background: Rectangle {
                            color: btn.hovered ? Qt.rgba(0, 0, 0,
                                                         0.12) : "transparent"
                        }
                        onClicked: {
                            popUp.clicked(index)
                            close()
                        }
                    }
                }
            }
        }
    }

    QtObject {
        id: innerObject
        property real requestKey: -1
    }

    function promptUser(key, title, description) {
        innerObject.requestKey = key
        titleText.text = title
        descriptionText.text = description
        open()
    }
    function generateKey() {
        return Math.random() * 1000
    }
}
