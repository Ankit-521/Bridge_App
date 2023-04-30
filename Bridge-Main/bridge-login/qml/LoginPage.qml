import QtQuick 2.14
import Bridge.Constants 1.0
import "./components"
import "./footers"
import "./controls"
import "./sections"
import QtQuick.Controls 2.12

Page {
    id: mainPage
    Connections {
        target: authManager
        onHandleError: {
            console.log("is erorr")
            if (errorMsg != "") {

                mainPage.state = ""
                dialog.promptUser(dialog.generateKey(), "Error",
                                  "Could not proceed because " + errorMsg)
            }
        }
    }

    AppButton {
        width: height
        height: 36
        anchors.right: parent.right
        anchors.rightMargin: 8
        y: 8

        Image {
            anchors.centerIn: parent
            mipmap: true
            width: height
            height: 24
            fillMode: Image.PreserveAspectFit
            smooth: true
            source: "../../common_assets/images/close.png"
        }
        onClicked: {
            authManager.closeWindow()
        }
    }

    // Main content section
    LoginContetSection {
        id: mainContentSection
        visible: (opacity > 0.1)
        opacity: 1
        scale: 1
    }

    // login busy section
    LoginBusySection {
        id: busySection
        opacity: 0
        scale: 0.95
        visible: (opacity > 0.1)
    }

    footer: LoginPageFooter {}

    // states
    states: [
        State {
            name: "loging"
            PropertyChanges {
                target: mainContentSection
                scale: 0.95
                opacity: 0
            }
            PropertyChanges {
                target: busySection
                opacity: 1
                scale: 1
            }
        }
    ]
    transitions: [
        Transition {
            from: ""
            to: "loging"
            reversible: true
            NumberAnimation {
                properties: "scale,opacity"
                duration: 400
                easing.type: Easing.InOutExpo
            }
        }
    ]

    PromptDialog {

        id: dialog
        titleItem.font.family: Constants.secondary
        titleItem.color: Constants.colors.primary
        descriptionItem.font.family: Constants.primary
        Component.onCompleted: {
            // hide the NO button
            repeater.itemAt(1).visible = false

            repeater.itemAt(0).text = "OK"
        }
    }
}
