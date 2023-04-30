import QtQuick 2.0
import Bridge.Constants 1.0
import QtQuick.Controls 2.5

// Item creates a smooth visual hoverable mouse item
Rectangle {
    property alias mousearea: mMouseArea
    id: mainParent
    color: Constants.colors.transparent
    MouseArea {
        id: mMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {

            mainParent.color = Qt.rgba(1, 1, 1, 0.1)
        }
        onExited: {

            mainParent.color = Qt.rgba(1, 1, 1, 0)
        }
    }

    // Behavior Animations
    Behavior on color {
        ColorAnimation {
            duration: 100
        }
    }
}
