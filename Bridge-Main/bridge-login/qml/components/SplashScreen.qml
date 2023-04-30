import QtQuick 2.14
import Bridge.Constants 1.0

Rectangle {
    anchors.fill: parent
    color: Constants.colors.primary
    Behavior on opacity {
        OpacityAnimator {
            duration: 300
        }
    }
    visible: (opacity > 0.1)
    Column {
        anchors.centerIn: parent
        Text {
            id: appName
            font.pixelSize: 32
            scale: 1.1
            opacity: 0
            font.family: Constants.primary

            color: Constants.colors.white
            text: authManager.isDirector ? qsTr("Bridge Club") : qsTr("Bridge")
        }
    }
    //version
    Row {
        id: copyrightRow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        spacing: 4
        opacity: 0

        Text {
            anchors.verticalCenter: parent.verticalCenter
            font.letterSpacing: 2
            font.pixelSize: 12
            font.family: Constants.light
            text: qsTr("1.20.3")
            color: "#c4dce0"
        }
    }
    states: [
        State {
            name: "show"
            PropertyChanges {
                target: appName
                scale: 1
                opacity: 1
            }
            PropertyChanges {
                target: copyrightRow
                opacity: 1
            }
        }
    ]
    transitions: [
        Transition {
            from: ""
            to: "show"
            reversible: false
            NumberAnimation {
                properties: "scale,opacity"
                easing.type: Easing.OutExpo
                duration: 3000
            }
        }
    ]

    function show() {
        opacity = 1
        state = "show"
    }
    function hide() {
        opacity = 0
    }
}
