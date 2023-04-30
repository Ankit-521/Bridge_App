import QtQuick 2.11
import QtQuick.Controls 2.12
import "./components"

// RoomControl that contains all videos of players
Popup {
    readonly property real myDialogKey: promptDialog.generateKey()
    id: mainPopup
    readonly property real expandedDelegateWidth: 470
    readonly property real expandedDelegateHeight: 300
    readonly property real shrinkedDelegateWidth: 316
    readonly property real shrinkedDelegateHeight: 200
    readonly property real minimumWidth: 350
    readonly property var sides: ["North", "East", "South", "West"]

    property var playerOffers: []

    property bool expanded: false
    padding: 0
    width: minimumWidth
    height: (canvas.height - statusBar.height)
    x: canvas.width - width
    y: 0
    closePolicy: Popup.NoAutoClose

    signal expanding
    Connections {
        target: canvas
        onWidthChanged: {
            if (expanded) {
                width = canvas.width
            }
        }
    }
    // Let it grow uniformly :)
    Behavior on width {
        NumberAnimation {
            easing.type: Easing.OutExpo
            duration: 400
        }
    }

    Connections {
        target: roommanager
        onOfferReceived: {
            console.log("an offer is receieced " + playerPosition + "  " + sides[me.num])
            if (me.num == playerPosition) {

                var data = {
                    "player": playerPosition,
                    "table": table,
                    "offer": offer
                }

                playerOffers.push(data)

                promptDialog.promptUser(
                            myDialogKey, "Invitation",
                            "Would you like join the tournament video chat room?")
            }
        }
    }

    background: Rectangle {
        id: popUpBackground
        color: Qt.rgba(0, 0, 0, 0.7)

        // Give it some life :)
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }
    }
    Column {
        id: mainCol
        anchors.horizontalCenter: parent.horizontalPadding
        width: parent.width
        x: parent.width / 2 - width / 2
        height: parent.height
        padding: 16
        spacing: 0

        // Controls Container
        // contains the close and expand controls of the Popup
        ControlContainer {
            id: controlContainer
        }

        // Header Container
        // contains the Header text
        RoomHeaderItem {
            id: roomHeader
        }

        Text {
            id: emptyPlayerText
            visible: (roomContentItem.mModel.count == 0)
            font.pixelSize: 20
            topPadding: 16
            font.family: "Arial"
            font.weight: Font.Light
            text: qsTr("No players yet")
            font.letterSpacing: 0.2
            color: Qt.rgba(1, 1, 1, 0.7)
        }

        // Content Item
        // Contains all players in the room(active players on the table)
        RoomContentItem {
            visible: !emptyPlayerText.visible
            id: roomContentItem
            width: parent.width
            height: mainCol.height - (roomHeader.height + (mainCol.padding * 2.3))
        }
    }

    function toggle() {

        if (opened) {
            close()

            // reset the popup Width
            reset()
        } else
            open()
    }

    function expand() {
        expanded = !expanded
        expanding()

        width = canvas.width
        mainCol.leftPadding = 128
        mainCol.rightPadding = 128
        //mainCol.width = 1000
        //  mainCol.x = (mainPopup.width / 2 - mainCol.width / 2)
        popUpBackground.color = Qt.rgba(0, 0, 0, 0.8)
    }

    function reset() {
        expanded = !expanded
        expanding()

        width = minimumWidth
        // mainCol.width = minimumWidth
        mainCol.leftPadding = 16
        mainCol.rightPadding = 16
        // mainCol.x = 0
        popUpBackground.color = Qt.rgba(0, 0, 0, 0.7)
    }
}
