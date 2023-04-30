import QtQuick 2.11
import QtQuick.Controls 2.2

import "./controls"
import "./headers"
import "./delegates"

Item {
    property real mTable: -1 // The table this page is displaying

    ErrorMessageItem {
        id: errorMsgItem
        visible: false //true
        title: "No players yet."
        description: ""
    }

    Flickable {
        // visible: !errorMsgItem.visible
        anchors.fill: parent
        contentHeight: mainColumn.height + 32 + myVideo.height
        Column {
            id: mainColumn
            width: parent.width - 32
            x: 16
            spacing: 16
            Repeater {
                model: 1
                delegate: MainVideoDelegate {}
            }
        }
    }
}
