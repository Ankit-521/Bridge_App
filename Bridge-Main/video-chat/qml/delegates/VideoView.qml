import QtQuick 2.12
import QtQuick.Controls 2.12
import "../controls"
import "../" as ParentDir

Rectangle {

    readonly property var mainWebEngineView: loader_mainWebEngineView.item
    property bool isMe: false
    color: Qt.rgba(0, 0, 0, 1)

    Component {
        id: component_mainWebEngineView
        ParentDir.MainWebEngineView {
            id: mMainWebEngineView
        }
    }

    /// Prevent the web engine loading from freezing the UI
    Loader {
        anchors.fill: parent
        id: loader_mainWebEngineView
        asynchronous: true
        sourceComponent: component_mainWebEngineView
    }

    Text {
        padding: 8
        anchors.bottom: parent.bottom
        font.pixelSize: 16
        font.family: "Arial"
        text: qsTr(isMe ? "You" : ("Player - " + model.side))
        font.letterSpacing: 0.5
        color: Qt.rgba(1, 1, 1, 0.9)
    }
    Button {
        id: invitationBtn
        visible: !isMe
        anchors.centerIn: parent
        text: "Invite"
        font.pixelSize: 16
        onClicked: {
            text = "Inviting...."
            mainWebEngineView.runJavaScript("invitePlayer(" + 0 /*(mTable - 1)*/
                                            + "," + 0 /*model.player*/
                                            + ")")
            enabled = false
        }
    }

    // Connections
    Connections {
        target: mainWindow
        onJsInvokeRequest: {
            console.log("a rewu su ssent s")
            if (player === model.player) {
                invitationBtn.text = "processing"
            }
        }
    }

    // helper function to call js functions
    function callJsFunc(func, callback) {
        mainWebEngineView.runJavaScript(func, callback)
    }
}
