import QtQuick 2.11
import QtQuick.Controls 2.12
import "../controls"

import QtWebEngine 1.1
import QtWebView 1.0
import QtWebChannel 1.0

// RoomControl that contains all videos of players
Rectangle {

    color: Qt.rgba(0, 0, 0, 1)

    Component.onCompleted: {
        width = 324
        height = 200
    }

    WebEngineView {
        id: mainWebEngineView
        //ancahors.fill: parent
        webChannel: WebChannel {
            id: mainWebChannel
            Component.onCompleted: {

                // expose the c++ class to the js engine
                //  registerObject("roommanager", roommanager)
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

                //  mainWebEngineView.grantFeaturePermission(securityOrigin,
                //  feature, true)
            }
        }
        onLoadingChanged: {

            // Ensure the html page is loaded successfully
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

    // listen for property changes from Popup
    Text {
        padding: 8
        anchors.bottom: parent.bottom
        font.pixelSize: 16
        font.family: "Arial"
        text: qsTr("Player - East")
        font.letterSpacing: 0.5
        color: Qt.rgba(1, 1, 1, 0.9)
    }
}
