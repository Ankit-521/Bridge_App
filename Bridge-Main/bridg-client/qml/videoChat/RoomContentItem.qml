import QtQuick 2.11
import QtQuick.Controls 2.12
import "./controls"
import "./delegates"

// RoomControl that contains all videos of players
Item {

    property alias mModel: roomModel

    clip: true

    ScrollView {
        id: mainFlickable
        width: parent.width
        height: parent.height
        // boundsBehavior: Flickable.StopAtBounds
        contentHeight: flowWrapper.height //mainContentFlow.height
        Item {
            id: flowWrapper
            width: parent.width
            height: mainContentFlow.height

            Flow {
                id: mainContentFlow
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 16
                topPadding: 16
                Repeater {
                    model: ListModel {
                        id: roomModel
                    }
                    delegate: ContentItemDelegate {}
                }
            }
        }
    }

    // Just to make it look more.....  :)
    Divider {
        anchors.bottom: parent.bottom
    }
    Connections {
        target: promptDialog
        onClicked: {
            if (promptDialog.key == myDialogKey && action == 0) {

                playerOffers.forEach(function (data) {
                    console.log("m  sta ad " + data + "  " + me.num)
                    roomModel.append({
                                         "position": "You - " + sides[data.player],
                                         "table": data.table,
                                         "isMe": (me.num == data.player),
                                         "offer": data.offer,
                                         "player": data.player
                                     })
                })
                toggle()
            }
        }
    }
}
