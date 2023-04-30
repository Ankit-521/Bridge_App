import QtQuick 2.11
import QtQuick.Controls 2.12
import "../controls"

import QtWebEngine 1.1
import QtWebView 1.0
import QtWebChannel 1.0

// RoomControl that contains all videos of players
Rectangle {
    readonly property real mDialogId: promptDialog.generateKey()
    color: Qt.rgba(0, 0, 0, 1)
    property bool isMe: false
    Component.onCompleted: {
        width = 324
        height = 200
    }

    WebEngineView {
        property var permissionData: ({})
        id: mainWebEngineView
        anchors.fill: parent
        webChannel: WebChannel {
            id: mainWebChannel
            Component.onCompleted: {
                // expose the c++ class to the js engine
                registerObject("roomManager", roommanager)
            }
        }
        onJavaScriptConsoleMessage: {
            console.log("my messgae us " + lineNumber + "  " + message)
        }
        url: "qrc:/../../html/videoView.html"
        onFeaturePermissionRequested: {
            console.log("im rwfafasdfsdfss  " + securityOrigin + " " + feature)

            // check if webpage is requesting access to user Camera and Audio devices
            if (feature == WebEngineView.MediaAudioVideoCapture
                    || feature == WebEngineView.MediaAudioCapture
                    || feature == WebEngineView.MediaVideoCapture) {

                permissionData.feature = feature
                permissionData.securityOrigin = securityOrigin
                promptDialog.promptUser(
                            mDialogId, "Permissions",
                            "Allow this app to access your camera and audio devices.")
            }
        }
        onLoadingChanged: {

            // Ensure the html page is loaded successfully
            if (loadRequest.status == WebEngineLoadRequest.LoadSucceededStatus) {
                console.log("is laode s")
                mainWebEngineView.runJavaScript("connect();")
                if (model.isMe) {
                    console.log('is me yes ')
                    mainWebEngineView.runJavaScript(
                                "setSide(" + model.player + ");")
                    mainWebEngineView.runJavaScript("useCamera();")
                }
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

    Behavior on height {
        NumberAnimation {
            easing.type: Easing.OutExpo
            duration: 400
        }
    }

    Text {
        padding: 8
        anchors.bottom: parent.bottom
        font.pixelSize: 16
        font.family: "Arial"
        text: model.position
        font.letterSpacing: 0.5
        color: Qt.rgba(1, 1, 1, 0.9)
    }

    //////////////// CONNECTIONS ////////////////////////////

    // listen for property changes from Popup
    Connections {
        target: mainPopup
        onExpanding: {
            if (expanded) {
                width = expandedDelegateWidth
                height = expandedDelegateHeight
            } else {
                width = shrinkedDelegateWidth
                height = shrinkedDelegateHeight
            }
        }
    }

    Connections {
        target: promptDialog
        onClicked: {
            if (promptDialog.key == mDialogId) {
                console.log("is acceptie s")
                mainWebEngineView.grantFeaturePermission(
                            mainWebEngineView.permissionData.securityOrigin,
                            mainWebEngineView.permissionData.feature,
                            (action == 0))
            }
        }
    }

    Connections {
        target: roommanager
        onPeerInitalisationComplete: {
            console.log("wfwefw we we w we w  we" + playerSide)
            if (playerSide == model.player) {

                console.log("handling offer")

                roommanager.javascriptData = {
                    "player": model.player,
                    "table": model.table,
                    "offer": model.offer
                }
                roommanager.invokeJsCallBack(0, roommanager.javascriptData)

                console.log("my daynas " + JSON.stringify(
                                roommanager.javascriptData))
                // handel user offer
                // mainWebEngineView.runJavaScript("handleOffer();")
            }
        }
    }
}
