import QtQuick 2.14
import Bridge.Constants 1.0
import "../components"
import "../footers"
import "../controls"
import QtQuick.Controls 2.12

Column {
    anchors.centerIn: parent
    spacing: 16

    // logo
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 42
        font.family: Constants.secondary
        color: Constants.colors.primary
        text: qsTr((authManager.isDirector ? " Bridge Club" : " Bridge"))
    }
    Text {
        font.pixelSize: 16
        font.family: Constants.primary
        color: Constants.colors.primary
        text: qsTr("Login into your account")
        anchors.horizontalCenter: parent.horizontalCenter
        leftPadding: 8
    }
    Repeater {
        id: textfielRepeater
        model: ListModel {
            Component.onCompleted: {
                append({
                           "placeHolder": "Username"
                       })

                append({
                           "placeHolder": "Password"
                       })
            }
        }
        delegate: Item {
            readonly property string text: textField.text
            anchors.horizontalCenter: parent.horizontalCenter
            width: 344
            height: mCol.height
            Column {
                id: mCol
                spacing: 4
                Text {
                    opacity: textField.focus ? 0.95 : 0.7
                    font.pixelSize: 16
                    font.family: Constants.primary
                    color: Constants.colors.primary
                    text: qsTr(model.placeHolder)
                    leftPadding: 8
                }
                TextField {
                    id: textField
                    width: 444
                    height: 56
                    font.pixelSize: 20
                    font.family: Constants.secondary
                    passwordMaskDelay: 200
                    color: Constants.primary
                    echoMode: index == 1 ? TextInput.Password : TextInput.Normal

                    background: Rectangle {
                        border.width: textField.focus ? 1.5 : 1.1
                        border.color: textField.focus ? Qt.rgba(0, 0, 0,
                                                                0.3) : Qt.rgba(
                                                            0, 0, 0, 0.15)
                        radius: 4
                    }
                }
            }
        }
    }
    CheckBox {
        id: control
        Component.onCompleted: {
            checked = true
        }
        font.family: Constants.primary
        font.pixelSize: 16
        width: 200
        text: "Keep me logged in"
        onCheckedChanged: {
            authManager.keepMeSignedIn(checked)
        }
        indicator: Rectangle {
            implicitWidth: 20
            implicitHeight: 20
            x: control.leftPadding
            y: parent.height / 2 - height / 2
            radius: 3
            border.width: control.checked ? 0 : 1.2
            border.color: control.down ? Qt.lighter(
                                             Constants.colors.primary) : Qt.rgba(
                                             0, 0, 0, 0.4)

            Rectangle {
                width: 14
                height: 14
                anchors.centerIn: parent
                radius: 2

                visible: control.checked
                Image {
                    fillMode: Image.PreserveAspectFit
                    width: height
                    anchors.centerIn: parent
                    height: 24
                    smooth: true
                    mipmap: true
                    source: "../../common_assets/images/check.png"
                }
            }
        }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 16
        Repeater {
            model: 1
            delegate: Button {
                id: button
                readonly property bool isGoogleBtn: false

                width: 200
                height: 42
                background: Rectangle {
                    color: button.isGoogleBtn ? Constants.colors.transparent : Constants.colors.primary
                    radius: 4
                    border.width: button.isGoogleBtn ? 1 : 0
                    border.color: button.isGoogleBtn ? Qt.rgba(
                                                           0, 0, 0,
                                                           0.2) : Constants.colors.transparent
                }
                contentItem: Text {
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.capitalization: Font.AllUppercase
                    font.letterSpacing: 1
                    color: Constants.colors.white
                    font.family: Constants.secondary
                    text: button.isGoogleBtn ? "" : qsTr("Login")
                }
                Row {
                    visible: button.isGoogleBtn
                    id: mRow
                    padding: 8
                    spacing: 8
                    anchors.centerIn: parent

                    Image {
                        fillMode: Image.PreserveAspectFit
                        width: height
                        height: 28
                        smooth: true
                        mipmap: true
                        source: "../../common_assets/images/google.png"
                    }
                    Text {
                        rightPadding: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 18
                        font.capitalization: Font.AllUppercase

                        color: Constants.colors.primary
                        font.family: Constants.secondary
                        text: qsTr(" gmail")
                    }
                }
                onClicked: {

                    mainPage.state = "loging"
                    switch (index) {
                    case 0:

                        // signin with gmail account
                        authManager.signIn(0)
                        break
                    case 1:
                        // signin with usernames and password
                        authManager.signIn(1, getTextAt(0), getTextAt(1))
                        break
                    }
                }
            }
        }
    }

    Row {
        topPadding: 16
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 9
        Text {
            font.pixelSize: 16
            font.family: Constants.primary
            color: Qt.lighter(Constants.colors.primary)
            text: qsTr("Don`t have an account?")
        }
        Link {
            font.pixelSize: 16
            font.family: Constants.secondary
            color: Constants.colors.primary
            text: qsTr("Create one now")
            link: "https://wfwf/fwefwef/"
        }
    }
    // functions
    function getTextAt(index) {
        return textfielRepeater.itemAt(index).text
    }
}
