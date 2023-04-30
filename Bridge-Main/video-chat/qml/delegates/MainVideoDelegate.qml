import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "../controls"

Item {

    width: parent.width
    height: mCol.height

    Column {
        id: mCol
        width: parent.width
        spacing: 0
        topPadding: 16
        Item {
            width: parent.width
            height: titleText.implicitHeight
            Text {
                id: titleText
                leftPadding: 16
                padding: 16
                width: parent.width
                bottomPadding: 8
                color: titleTextColor
                font.pixelSize: 40
                font.family: "arial"
                text: qsTr("Table " + (mTable))
            }
        }
        Item {
            width: parent.width
            height: subFlow.height
            // color: "brown"
            GridLayout {
                id: subFlow
                width: parent.width
                columns: 2
                Repeater {
                    id: mPlayerRepeater
                    model: 1
                    ListModel {
                        id: playerModel
                    }
                    delegate: VideoView {
                        Layout.preferredWidth: 350
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredHeight: 250
                        color: Qt.rgba(0, 0, 0, 1)
                    }
                }
            }
        }
    }
    Connections {
        target: mainWindow
        onNewPlayerJoined: {

            console.log("dwedwefwev  " + mTable + "  " + (table + 1) + "  " + joined)
            if (joined) {
                if ((table + 1) == (mTable)) {

                    playerModel.append({
                                           "side": sides[playerPosition],
                                           "player": playerPosition
                                       })
                    console.log("a new player has joine d now " + table + " " + playerPosition)

                    // notify director of new players
                    var tabButton = tablePageHeader.tabButtonsRepeater.itemAt(
                                table)
                    var prevCount = tabButton.currentNewPlayers
                    prevCount++
                    tabButton.currentNewPlayers = prevCount
                }
            } else {
                console.log("im remove efreb  " + playerPosition)
                for (var i = 0; i < playerModel.count; i++) {
                    var player = playerModel.get(i).player

                    console.log("player is  " + player)
                    if (player == playerPosition) {
                        console.log("removi s player " + player + " at positi  " + playerPosition)
                        playerModel.remove(i)
                        break
                    }
                }
            }
            errorMsgItem.visible = (playerModel.count == 0)
        }
    }
}
