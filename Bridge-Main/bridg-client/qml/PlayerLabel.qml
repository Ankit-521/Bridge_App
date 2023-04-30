import QtQuick 2.14

Column {
    property Item player
    readonly property bool isMyTurn: (turn == player.num && dummy != player.num)
    spacing: 2
    visible: phase > 0 && phase < 3
    anchors {
        bottom: player.top
        bottomMargin: 5
        left: player.left
        leftMargin: 10
    }
    Text {

        text: ["North", "East", "South", "West"][player.num] + " (Pair "
        + (player.num % 2 ? ewPair : nsPair) + ")"
        font.pixelSize: 16
        color: "white"
    }
    Text {
        id: bidText
        text: "bidding"
        font.letterSpacing: 0.3
        font.pixelSize: 10
        color: Qt.rgba(1, 1, 1, 0.85)
    }

    // Bid timer
    Timer {
        running: true
        id: bidTimer
        repeat: true
        interval: 1000 // every second
        onTriggered: {
            var _bidTime = time_obje.getTime()
            bidText.text = "bidding - " + _bidTime
        }
    }

    // encapsulate time handles
    QtObject {
        id: time_obje
        property real seconds: 0
        property real minutes: 0
        property real hours: 0

        // constructs and returns time since a bid begun
        function getTime() {
            if (seconds == 60) {

                seconds = 0
                minutes++
                if (minutes == 60) {
                    hours++
                    minutes = 0
                    if (hours == 24) {
                        hours = 0
                    }
                }
            }
            seconds++
            return polish(minutes) + ":" + polish(seconds)
        }

        // resets time handles
        function reset() {
            minutes = 0
            hours = 0
            seconds = 0
        }

        // Polishes time handles
        function polish(handle) {
            var targetTime = handle
            if (handle <= 9) {
                targetTime = "0" + handle
            }
            return targetTime
        }
    }

    /// Connections
    Connections {
        target: canvas
        onTurnChanged: {

            // Check if is dummy partner
            var isDummyPartner = (turn == dummy && dummy == (player.num + 2) % 4
                                  && phase == 2)

            if (isDummyPartner || isMyTurn) {

                bidTimer.start()
            } else {

                bidTimer.stop()
                time_obje.reset()
                bidText.text = "waiting..."
            }
        }
    }
}
