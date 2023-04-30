import QtQuick 2.0
import Bridge.Constants 1.0
import QtQuick.Controls 2.5
import "../headers"

Page {
    anchors.fill: parent

    SwipeView {
        id: mainSwipeView
        anchors.fill: parent
        interactive: false
        MatchCreatorPage {}
        RoomPage {}
    }
}
