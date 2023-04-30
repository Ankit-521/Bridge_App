import QtQuick 2.14
import Bridge.Constants 1.0
import QtQuick.Controls 2.12

Button {
    background: Rectangle {
        radius: width / 2
        Behavior on color {
            ColorAnimation {
                duration: 100
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: parent.clicked()
        hoverEnabled: true

        onPressed: {
            background.color = Qt.rgba(0, 0, 0, 0.2)
        }
        onReleased: {
            background.color = Constants.colors.white
        }
        onEntered: {
            background.color = Constants.colors.secondary
        }

        onExited: {
            background.color = Constants.colors.white
        }
    }
}
