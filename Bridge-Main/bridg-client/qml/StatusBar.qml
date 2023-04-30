import QtQuick 2.14

Item {
    id: bar
    height: 32

    property int round: 0
    property int table: -1
    property int boardNo
    property alias text: statusText.text

    property alias showText: statusText.visible
    property alias showNewGameBtn: newGameBtn.visible
    property alias showCloseBtn: closeBtn.visible
    property alias bgColor: backgrounds.currentColor
    signal settingsTriggered
    signal newGame
    signal closed

    Rectangle {
        id: background
        color: "black"
        opacity: 0.4
        anchors.fill: parent
    }

    Row {
        id: backgrounds
        anchors {
            left: parent.left
            leftMargin: 20
        }
        height: parent.height
        property string currentColor: "green"
        spacing: 2

        Repeater {
            model: ListModel {
                ListElement {
                    color: "green"
                }
                ListElement {
                    color: "brown"
                }
                ListElement {
                    color: "red"
                }
            }
            Image {
                source: "images/backgrounds/" + color + "_thumb.png"
                height: parent.height
                fillMode: Image.PreserveAspectFit
                smooth: true
                MouseArea {
                    anchors.fill: parent
                    onClicked: backgrounds.currentColor = color
                }
            }
        }
    }

    Text {
        id: roundBoard
        text: " Table: " + parent.table + ", Board:" + parent.boardNo + ", Round:" + parent.round
        color: "white"
        anchors {
            verticalCenter: parent.verticalCenter
            left: backgrounds.right
            leftMargin: 10
        }
        font.pixelSize: 14
        visible: parent.round > 0
    }

    Text {
        id: statusText
        color: "white"
        anchors.centerIn: parent
        font.pixelSize: 14
    }

    Row {
        anchors.centerIn: parent
        Button {
            id: newGameBtn
            text: "New Game"
            onClicked: bar.newGame()
        }
        Button {
            id: closeBtn
            text: "Close"
            onClicked: bar.closed()
        }
    }


    /*
*
* Code changed by Wasswa Geoffrey on 03/11/2021
*
*/


    /*   Rectangle {
        id: iconHighlight
        color: "white"
        opacity: 0.5
        radius: 5
        anchors.fill: settingsIcon
        visible: mouseArea.pressed
    }

    Image {
        id: settingsIcon
        source: "images/settings.png"
        anchors {
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
            bottomMargin: 5
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: bar.settingsTriggered()
        }
    }
*/
    Item {
        id: rightSettingsContainer
        width: settingsRow.width
        height: settingsRow.height
        anchors {
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
            bottomMargin: 5
        }
        Row {
            id: settingsRow
            spacing: 8
            rightPadding: 16

            //  display the tournament uptime
            Text {
                id: tournament_uptime
                color: "white"
                anchors {
                    verticalCenter: parent.verticalCenter
                }
                font.pixelSize: 14
            }

            /// Show settings controls
            Repeater {
                model: ListModel {
                    Component.onCompleted: {
                        append({
                                   "icon": "images/video.png"
                               })
                        append({
                                   "icon": "images/settings.png"
                               })
                    }
                }
                delegate: Rectangle {
                    id: iconHighlight
                    color: mouseArea.pressed ? Qt.rgba(1, 1, 1,
                                                       0.7) : Qt.rgba(1, 1, 1,
                                                                      0)
                    opacity: 0.5
                    radius: 5
                    anchors.verticalCenter: parent.verticalCenter
                    width: settingsIcon.width
                    height: settingsIcon.height

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        onClicked: {
                            switch (index) {
                            case 0:
                                console.log("is opene s")
                                roomControl.toggle()
                                break
                            case 1:
                                bar.settingsTriggered()
                                break
                            }
                        }
                    }
                    Image {
                        id: settingsIcon
                        width: 24
                        height: 24
                        source: model.icon
                        anchors {
                            centerIn: parent
                        }
                    }
                }
            }
        }
    }

    /// Connections
    Connections {
        target: talker
        onTournamentUptimeChanged: {
            tournament_uptime.text = uptime
          }
    }
}
