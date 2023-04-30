import QtQuick 2.11
import QtQuick.Controls 2.12

import QtWebEngine 1.1
import QtWebView 1.0
import QtWebChannel 1.0

/// Main Web view Item
WebEngineView {
    property alias mainWebChannel: inner_mainWebChannel

    id: mainWebEngineView
    Component.onCompleted: {

    }
    anchors.fill: parent
    webChannel: WebChannel {
        id: inner_mainWebChannel
        Component.onCompleted: {
            registerObject("mainWindow", mainWindow)
        }
    }
    onJavaScriptConsoleMessage: {

        // for logging purposes
        console.log("my messgae us " + lineNumber + "  " + message)
    }
    url: "qrc:/../../html/videoView.html"
    onFeaturePermissionRequested: {
        console.log("im rwfafasdfsdfss  " + securityOrigin + " " + feature)

        // check if webpage is requesting access to user Camera and Audio devices
        if (feature == WebEngineView.MediaAudioVideoCapture
                || feature == WebEngineView.MediaAudioCapture
                || feature == WebEngineView.MediaVideoCapture) {
            console.log("im graifntins ")
            /// grant camera access for the mean time
            mainWebEngineView.grantFeaturePermission(securityOrigin,
                                                     feature, true)
        }
    }
    onLoadingChanged: {
        console.log("dwefwe  " + loadRequest.errorString)
        // Ensure the html page is loaded successfully
        if (loadRequest.status == WebEngineLoadRequest.LoadSucceededStatus) {

            // mainWebEngineView.runJavaScript(mainWindow.qtWebChannelJs)
            // mainWebEngineView.runJavaScript(mainWindow.clientJs)
            // console.log("ismloafins")
            // connect the mainwindow object to js
            // callJsFunc("connect();")

            // request and use camera and audio if this video view is mine
            if (isMe) {

                //    callJsFunc("getCamera();")
            }
        }
    }
}
