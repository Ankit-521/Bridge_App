import QtQuick 2.14
import Bridge.Constants 1.0
import QtQuick.Controls 2.5
import "../headers"
import "../headers"
import "../controls"
import "../components"

Item {
    id: mainItem
    readonly property real itemWidth: mainItem.width - (mainColumn.leftPadding * 2)
    ScrollView {
        anchors.fill: parent
        Column {
            id: mainColumn
            width: parent.width
            spacing: 8
            padding: 80
            topPadding: 40
            BigHeader {
                text: qsTr("Room")
            }
            Text {
                font.pixelSize: 16
                font.letterSpacing: 0
                font.family: Constants.primary
                color: Constants.colors.primary
                text: qsTr("Browse and choose your players")
            }

            Repeater {
                model: 5
                delegate: Rectangle {
                    width: itemWidth
                    height: mCol.height
                    Column {
                        id: mCol
                        width: parent.width
                        spacing: 16
                        bottomPadding: 32
                        topPadding: bottomPadding
                        BigHeader {
                            font.pixelSize: 24
                            font.family: Constants.secondary
                            text: "Highest points"
                        }
                        Flow {
                            width: parent.width
                            spacing: 8
                            Repeater {
                                model: 15
                                delegate: PlayerComponent {
                                    subText.text: ((index + 15) + " points")
                                    titleText.text: "Player"
                                    mousearea.onClicked: {
                                        player_details.open()
                                    }
                                }
                            }
                        }
                    }
                    Divider {
                        width: parent.width
                        y: parent.height - height
                    }
                }
            }
        }
    }

    // Display player details
    PlayerDetails {
        id: player_details
        //  y: mainPage.header.height
    }
}
