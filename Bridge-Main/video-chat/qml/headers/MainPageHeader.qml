import QtQuick 2.11
import QtQuick.Controls 2.2

import "../controls"

Rectangle {
    property alias tabButtonsRepeater: tabTableRepeater
    width: parent.width
    height: headerCol.height
    Divider {
        x: 16
        width: parent.width - 32
        anchors.bottom: parent.bottom
    }

    Column {
        id: headerCol
        width: parent.width
        spacing: 8
        padding: 16
        bottomPadding: 4
        leftPadding: 0
        Text {
            width: parent.width
            color: titleTextColor
            font.pixelSize: 38
            font.weight: Font.Light
            font.family: "arial"
            font.letterSpacing: 1
            leftPadding: 16
            text: qsTr("Tournament")
        }

        TabBar {
            id: tabControl
            x: -8
            width: parent.width

            Repeater {
                id: tabTableRepeater
                model:mainWindow.tables
                delegate: TabButton {
                    property var currentNewPlayers: 0
                    onClicked: {
                        mainSwipeView.currentIndex = index
                        currentNewPlayers = 0
                    }
                    id: tabButton
                    width: 100
                    height: 36
                    text: "Table " + (index + 1)
                    Rectangle {
                        id: mPlayerCounter
                        width: height
                        visible: (!(currentNewPlayers == 0)
                                  && !(index == mainSwipeView.currentIndex))
                        height: 16
                        color: Qt.rgba(0, 0, 0, 1)
                        radius: width / 2
                        anchors.right: parent.right
                        anchors.top: parent.top
                        Text {
                            id: name
                            text: currentNewPlayers
                            font.family: "arial"
                            color: Qt.rgba(1, 1, 1, 1)
                            anchors.centerIn: parent
                            font.pixelSize: 10
                        }
                    }
                    contentItem: Text {
                        id: contentText
                        font.letterSpacing: 0.4
                        font.pixelSize: 16
                        opacity: hovered ? 1 : ((index == mainSwipeView.currentIndex) ? 1 : 0.4)

                        color: titleTextColor
                        text: tabButton.text
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        Rectangle {
                            width: tabButton.contentItem.implicitWidth
                            height: 3
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "brown"
                            opacity: index == mainSwipeView.currentIndex ? 1 : 0
                        }
                    }
                }
            }
        }
    }
}
