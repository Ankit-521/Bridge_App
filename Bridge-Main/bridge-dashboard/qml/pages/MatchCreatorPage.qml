import QtQuick 2.12
import Bridge.Constants 1.0
import QtQuick.Controls 2.5
import "../headers"
import "../controls"
import "../components"

Item {
    id: mainItem
    property var sides: ["North", "East", "South", "West"]
    ScrollView {
        anchors.fill: parent
        contentWidth: 0
        Column {
            id: mainColumn
            width: parent.width
            spacing: 8
            padding: 80
            topPadding: 40
            Text {
                font.pixelSize: 40
                font.letterSpacing: 0
                font.family: Constants.light
                color: Constants.colors.primary
                text: qsTr("Tournament")
                lineHeight: 0.8
            }
            Text {
                id: tournamentState
                font.pixelSize: 16
                font.letterSpacing: 0
                font.family: Constants.primary
                color: Constants.colors.primary
                text: qsTr("No running tournament yet")
            }
            Text {
                id: tournamentUptime
                font.pixelSize: 40
                font.letterSpacing: 0
                font.family: Constants.light
                color: Constants.colors.primary
                text: qsTr("__:__:__")
                lineHeight: 0.8
                bottomPadding: 16
            }
            Divider {
                width: mainItem.width - (mainColumn.leftPadding * 2)
            }
            Text {
                font.pixelSize: 40
                font.letterSpacing: 0
                font.family: Constants.light
                color: Constants.colors.primary
                text: qsTr("Players")
                padding: 0
                lineHeight: 0.8
            }
            Text {
                font.pixelSize: 16
                font.letterSpacing: 0
                font.family: Constants.primary
                color: Constants.colors.primary
                text: qsTr("Create a match by choosing your the players")
            }

            Flow {
                width: mainItem.width - (mainColumn.leftPadding * 2)
                topPadding: 16
                spacing: 64
                Repeater {
                    id: sideRepeater
                    model: ListModel {
                        Component.onCompleted: {
                            for (var i = 0; i < sides.length; i++)
                                append({
                                           "player": ("Player - " + sides[i]),
                                           "subText": "disconnected"
                                       })
                        }
                    }
                    delegate: PlayerComponent {
                        titleText.text: model.player
                        subText.text: model.subText
                    }
                }
            }
        }
    }

    // Connections
    Connections {
        target: serverListner
        onConnected: {
            console.log("server is connected ")
        }
        onDisconnected: {

            console.log("disconnec dform srh ese")
        }
        onTournamentUptimeChanged: {
            tournamentState.text = "Running..."
            tournamentUptime.text = uptime
        }
        onPlayerConnected: {
            console.log("a new player is connected " + player)
            sideRepeater.itemAt(player).subText.text = "Connected"
        }
        onPlayerMoved: {
            for (var i = 0; i < sides.length; i++) {
                sideRepeater.itemAt(i).subText.text = "waiting...."
                if (player == i) {
                    sideRepeater.itemAt((i )).subText.text = "bidding"
                }
            }
        }
    }
}
