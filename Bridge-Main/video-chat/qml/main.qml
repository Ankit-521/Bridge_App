import QtQuick 2.11
import QtQuick.Controls 2.2

import "./controls"
import "./headers"
import "./delegates"

Item {
    readonly property color titleTextColor: "#212121"
    readonly property color subTextColor: "#616161"
    readonly property var sides: ["North", "East", "South", "West"]
    ErrorMessageItem {}

    Page {
        id: mainPage
        visible: !(mainWindow.tables == 0)
        anchors.fill: parent
        padding: 0
        header: MainPageHeader {
            id: tablePageHeader
        }
        SwipeView {
            id: mainSwipeView
            anchors.fill: parent
        }
    }

    Component {
        id: tableComp
        TableContent {}
    }

    Connections {
        target: mainWindow
        onTablesChanged: {

            for (var i = 0; i < mainWindow.tables; i++) {
                var item = Qt.createComponent("TableContent.qml").createObject()

                // bind table index to the object
                item.mTable = Qt.binding(function () {
                    return (i + 1)
                })

                mainSwipeView.addItem(item)
            }
        }
        onEventCancelled: {
            while (mainSwipeView.count > 0) {
                mainSwipeView.takeItem(0).destroy(0)
            }
        }
        onErrorChanged: {
            console.log("my error is  " + error + "  " + errorMsg)
        }
    }
    Rectangle {
        id: myVideo
        visible: mainPage.visible
        width: 250
        height: 150
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.bottomMargin: 16
        color: "black"
        Loader {
            anchors.fill: parent
            asynchronous: true
            sourceComponent: Component {
                VideoView {
                    isMe: true
                }
            }
        }
    }
}
