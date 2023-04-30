import QtQuick 2.14
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import "logic.js" as Logic
import QtQuick.Dialogs 1.2
import "./videoChat"
import "./videoChat/controls"

//import Qt.labs.qmlmodels 1.0
Image {
    id: canvas
    source: "images/backgrounds/" + statusBar.bgColor + ".jpg"
    smooth: true

    // Code added by wasswa Geoffrey
    width: 800
    height: 600
    property bool animated: true

    property int dealer: -1
    property int vulnerable: -1
    property int declarer: -1
    property int doubled: 1
    property int turn: -1
    property int dummy: -1
    property int phase: 0
    property int nsPair: -1
    property int ewPair: -1

    property alias coverNo: settings.coverNo
    property alias deckNo: settings.deckNo
    property alias pickCard: pickCardCheck.checked

    property bool spectator

    signal newGame
    signal closed
    property variant win

    // PopUp window when alert is received "ankit"
    function setAnimated(value) {
        animated = value
    }

    function setBoard(dealer, vulnerable, table, round, boardNo, ns_pair, ew_pair) {
        canvas.dealer = dealer
        canvas.vulnerable = vulnerable
        statusBar.round = round
        statusBar.table = table
        statusBar.boardNo = boardNo
        canvas.nsPair = ns_pair
        canvas.ewPair = ew_pair
        scoreMessage.text = ""
    }

    function showCards(player, cards) {
        Logic.showCards(player, cards)
    }
    function showMove(player, type, data) {
        Logic.showMove(player, type, data)
    }
    function showDummyCards(cards) {
        Logic.showDummyCards(cards)
    }

    // showing "PopUp.qml when alert is received"   "ankit"
    function showNotification(alert) {
        var component = Qt.createComponent("PopUp.qml")
        win = component.createObject(this)
        win.alert = alert
        win.show()
    }

    // ankit
    Component.onCompleted: {
        if (false) {
            me.num = 2
            spectator = false
            Logic.updateDB()
            setBoard(1, 0, 1, 12)
            showCards(2,
                      ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"])
            showMove(1, 0, "1,0")
            showMove(2, 0, "pass")
            showMove(3, 0, "pass")
            showMove(0, 0, "pass")
        }
    }

    Connections {
        target: talker
        onTournamentUptimeChanged: {

            //   console.log("my uptime is  " + uptime)
        }
        onPlayerNumber: {
            spectator = num > 3
            if (!spectator)
                me.num = num
            else
                me.num = 2 // set spectator view to show south player at bottom
            Logic.updateDB()
            scoreMessage.text = "Waiting for other players..."
        }
        onBoardReceived: {
            console.log("a board is recievedv  " + me.num)
            setBoard(dealer, vulnerable, table, round, boardNo,
                     ns_pair, ew_pair)
        }
        onCardsReceived: showCards(player, cards)
        onDummyCardsReceived: showDummyCards(cards)
        onNotificationReceived: {
            for (var i in pointsList) {
                var total = 0
                for (var j in pointsList[i].split(',').slice(1))
                    total += pointsList[i].split(',').slice(1)[j]


                /* By chanchal Dass 05012021
                 tableView.model.appendRow({"Pair":i,
                                      "Total": total})
            }
            */
                //By chanchal Dass 05012021
            }
            if (note.indexOf("Tournament over") !== -1) {
                phase = 4
            }
            scoreMessage.text += "\n\n" + note
        }
        // ankit
        onAlertnotificationReceived: showNotification(
                                         alert) // alert is received in qml "ankit"
        onClaimReceived: {
            claimDialog.msg = claim_msg
            claimDialog.num = claim_num
            claimDialog.fromplayer = claim_player
            claimDialog.visible = true
        }
        onClaimresponseReceived: {

            // if my claim is accpeted
            if (playerTo == me.num) {
                claimacceptDialog.visible = true
                claimacceptDialog.response = response
            }
            // opponent other than who has rejected or accepted claim
            if (((me.num == (playerTo + 1) % 4 || me.num == (playerTo + 3) % 4))
                    && claimDialog.visible == true) {
                claimDialog.visible = false
                claimresponseDialog.visible = true
                claimresponseDialog.msg = "claim is " + response
            }

            if (response == "accepted") {
                Logic.endbyclaim(num_tricks, (playerTo == me.num
                                              || (playerTo + 2) % 4 == me.num))
            }
        }
    }

    //ankit
    Dialog {
        id: claimresponseDialog
        title: "claim"
        property string msg: ""
        visible: false
        Text {
            text: claimresponseDialog.msg
        }
    }

    Dialog {
        id: claimDialog
        visible: false
        property int num: -1
        property string msg: ""
        property int fromplayer: -1
        title: "Do you agree with opponent's claim?"
        Text {
            text: "Number of tricks claimed by opponent: " + claimDialog.num
                  + "\n" + "Explanation given: " + claimDialog.msg
        }
        standardButtons: Dialog.Yes | Dialog.No
        onYes: {
            //agreeing
            //end board and give results
            talker.broadcastClaimResponse(1, claimDialog.fromplayer,
                                          "accepted", claimDialog.num)
        }
        onNo: {
            //challenging
            //continue the game
            talker.broadcastClaimResponse(1, claimDialog.fromplayer,
                                          "rejected", claimDialog.num)
        }
    }
    Dialog {
        id: claimacceptDialog
        title: "claim"
        visible: false
        property string response: ""
        Text {

            text: claimacceptDialog.response
        }
    }

    //ankit
    MouseArea {
        anchors.fill: parent
        onClicked: settings.height = 0
    }

    Timer {
        id: sendDummyCardsTimer
        interval: 300
        onTriggered: {
            talker.broadcastDummyCards()
            Logic.dummyCardsOpen = true
        }
    }

    Timer {
        id: turnTimer
        property int nextTurn: -1
        interval: moveCardsBackTimer.interval
        onTriggered: canvas.turn = nextTurn
    }

    Chat {
        id: chat

        x: 10
        y: leftPlayer.y + leftPlayer.height - 10
        z: 1

        height: 220
        width: 500

        onSendMessage: talker.broadcastMessageChat(type, message)
    }

    // Click on F2 for show the Chat
    // Click on Escape to hide to chat
    Shortcut {
        sequences: ["F2"]
        onActivated: chat.isVisibleChat = true
    }

    function addMessageChat(type, message, sender) {
        Logic.addMessageChat(type, message, sender)
    }

    StatusBar {
        id: statusBar
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        showText: phase > 0 && phase < 3 && !spectator
        showNewGameBtn: phase == 3
        showCloseBtn: spectator || phase == 4
        onSettingsTriggered: {
            settings.height = settings.height > 0 ? 0 : 150
        }
        onNewGame: canvas.newGame()
        onClosed: canvas.closed()
    }
    // these are required because some script might set another
    // message to show (eg, invalid move), after that the turn
    // messages won't be automatically updated without the
    // following 2 lines
    onTurnChanged: {
        statusBar.text = turnMessage(turn, phase)
        console.log("a turn is changed " + turn + "  " + phase)
    }
    onPhaseChanged: {
        console.log("a phase is changed " + turn + " " + phase)
        statusBar.text = turnMessage(turn, phase)
    }
    function turnMessage(turn, phase) {
        if (turn == me.num && dummy != me.num)
            return phase == 1 ? "Your turn to bid" : "Your turn to play a card"
        if (turn == dummy && dummy == (me.num + 2) % 4 && phase == 2)
            return "Your turn to play dummy's card"
        return ""
    }

    Item {
        id: container
        height: parent.height
        width: height * 4 / 3
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Image {
        id: centerShadow
        source: "images/Center_Shadow.png"
        anchors {
            centerIn: parent
            bottomMargin: statusBar.height
        }
        visible: phase > 1
    }

    Display {
        id: display
        visible: phase > 0
        anchors {
            top: parent.top
            topMargin: 5
            left: parent.left
            leftMargin: parent.width * 5 / parent.height
        }

        dealer: canvas.dealer
        vulnerable: canvas.vulnerable
        declarer: phase > 1 ? canvas.declarer : -1
        contract: (phase > 1) ? ((Logic.lastValidBid !== "") ? Logic.lastValidBid.split(
                                                                   ":")[1] : "") : ""
        doubled: phase > 1 ? canvas.doubled : 0
    }

    BiddingBoard {
        id: biddingBoard
        anchors {
            top: topPlayer.bottom
            topMargin: 15
            horizontalCenter: me.horizontalCenter
        }
        height: 200
        width: 400
        visible: false
        opacity: visible ? 1 : 0
        Behavior on opacity {
            NumberAnimation {
                duration: 250
            }
        }
    }

    BiddingBox {
        id: biddingBox
        anchors {
            bottom: me.top
            bottomMargin: 30
            horizontalCenter: me.horizontalCenter
        }
        width: choice == 1 ? 370 : 450
        height: 170
        opacity: phase == 1 && turn == me.num && !spectator
        Behavior on opacity {
            NumberAnimation {
                duration: 250
            }
        }

        onPlayerbid: {
            console.log("player bid ")
            if (Logic.validateBid(bidNo, suit, special)) {
                // if textalert is non-empty then alert is pressed by the player "ankit"
                if (textalert) {
                    var bid = special == "" ? bidNo + "," + suit : special

                    talker.broadcastAlert(0, bid, textalert)
                    showMove(me.num, 0, bid)
                } else {
                    bid = special == "" ? bidNo + "," + suit : special
                    talker.broadcastMove(0, bid)
                    showMove(me.num, 0, bid)
                }
            }
            // ankit
        }
    }

    Timer {
        id: moveCardsBackTimer
        interval: 1000 * animated
        onTriggered: Logic.moveCardsToPlayers()
    }

    ScoreBoard {
        id: scoreBoard
        anchors {
            top: leftPlayer.bottom
            topMargin: 20
            left: parent.left
            leftMargin: 30
            bottom: statusBar.top
            bottomMargin: 10
            right: me.left
            rightMargin: 20
        }
        visible: phase > 1
        opacity: visible ? 1 : 0
    }

    Timer {
        id: stopBidTimer
        interval: 1000 * animated
        onTriggered: {
            biddingBoard.visible = biddingBox.visible = false
        }
    }

    Rectangle {
        color: "black"
        property int margin: -5
        anchors {
            fill: scoreMessage
            topMargin: margin
            bottomMargin: margin
            leftMargin: margin * 2
            rightMargin: margin * 2
        }
        opacity: visible ? 0.7 : 0
        radius: 7
        visible: scoreMessage.visible
        Behavior on opacity {
            NumberAnimation {
                duration: 250
            }
        }
    }
    Text {
        id: scoreMessage
        anchors {
            horizontalCenter: me.horizontalCenter
            bottom: me.top
            bottomMargin: 40
        }
        font.pixelSize: 20
        color: "white"
        visible: text != ""
        opacity: visible
        Behavior on opacity {
            NumberAnimation {
                duration: 250
            }
        }
    }

    Rectangle {
        color: "black"
        anchors {
            fill: tableView
        }
        opacity: visible ? 0.7 : 0
        radius: 7
        visible: tableView.visible
        Behavior on opacity {
            NumberAnimation {
                duration: 250
            }
        }
    }
    TableView {
        id: tableView
        //            width:200
        //            height:200
        anchors.fill: biddingBoard
        columnSpacing: 1
        rowSpacing: 1
        visible: phase === 4
        anchors {
            bottom: scoreMessage.top
            bottomMargin: 40
        }


        /*
            model: TableModel {
                TableModelColumn { display: "Pair" }
                TableModelColumn { display: "Total" }


                rows: [
                    {
                        "Pair": "Pair",
                        "Total": "Total"
                    },

                ]
            }
*/
        delegate: Rectangle {
            implicitWidth: 100
            implicitHeight: 50
            border.width: 1

            Text {
                text: display
                anchors.centerIn: parent
            }
        }
    }

    Timer {
        id: scoreTimer
        interval: (moveCardsBackTimer.interval + 100) * animated
        onTriggered: Logic.displayScore()
    }

    PlayerLabel {
        player: topPlayer
    }
    Player {
        id: topPlayer
        num: (me.num + 2) % 4
        orientation: Qt.Horizontal
        pickDirection: 1
        anchors {
            top: parent.top
            topMargin: 30
            horizontalCenter: parent.horizontalCenter
        }
        mouseEnabled: phase == 2 && topPlayer.num == dummy
                      && turn == topPlayer.num && settings.height == 0
                      && !spectator

        onCardClicked: {
            if (Logic.validateMove(num, parseInt(cardNoString))) {
                talker.broadcastMove(2, indexString)
                showMove(me.num, 2, indexString)
            } else {
                statusBar.text = "You cannot play that card!"
            }
        }

        function isValid(cardNo) {
            return Logic.validateMove(num, cardNo)
        }
    }

    PlayerLabel {
        player: leftPlayer
    }
    Player {
        id: leftPlayer
        num: (me.num + 1) % 4
        orientation: Qt.Vertical
        anchors {
            left: container.left
            leftMargin: 40
            verticalCenter: parent.verticalCenter
        }
    }

    PlayerLabel {
        player: rightPlayer
    }
    Player {
        id: rightPlayer
        num: (me.num + 3) % 4
        orientation: Qt.Vertical
        anchors {
            right: container.right
            rightMargin: 30
            verticalCenter: parent.verticalCenter
        }
    }

    PlayerLabel {
        player: me
    }
    Player {
        id: me

        orientation: Qt.Horizontal
        pickDirection: -1
        anchors {
            bottom: statusBar.top
            bottomMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        mouseEnabled: phase == 2 && me.num != dummy && turn == me.num
                      && settings.height == 0 && !spectator

        onCardClicked: {
            if (Logic.validateMove(me.num, parseInt(cardNoString))) {
                talker.broadcastMove(1, cardNoString)
                showMove(me.num, 1, indexString)
            } else {
                statusBar.text = "You cannot play that card!"
            }
        }

        function isValid(cardNo) {
            return Logic.validateMove(me.num, cardNo)
        }
    }

    CheckBox {
        id: pickCardCheck
        text: "Pick card before playing"
        checked: true
        visible: phase > 1 && phase < 3 && !spectator
        anchors {
            left: me.right
            leftMargin: 20
            bottom: me.bottom
            bottomMargin: 5
        }
    }

    SettingsBox {
        id: settings
        width: parent.width
        height: 0
        anchors {
            bottom: statusBar.top
        }
    }
    //ankit claim
    Button {
        id: claim
        visible: (((turn === me.num || topPlayer.mouseEnabled) && phase === 2)
                  && Logic.lastCard === -1)
        text: "<font color=\"red\">Claim</font>"
        anchors {
            bottom: pickCardCheck.top
            topMargin: 5
            leftMargin: 20
            left: me.right
        }
        width: 50

        onClicked: {
            claimbox.visible = true
            claimbox.tricks_left = Logic.cards(me.num).count
        }
    }

    Claim {
        id: claimbox
        visible: false
        anchors {
            bottom: pickCardCheck.top
            bottomMargin: 30
            leftMargin: 20
            left: me.right
        }
        onMakeclaim: {
            talker.broadcastClaim(1, claim_num, claim_msg)
        }
    }

    // Main room control item
    RoomControl {
        id: roomControl
    }

    PromptDialog {
        id: promptDialog
    }
}
