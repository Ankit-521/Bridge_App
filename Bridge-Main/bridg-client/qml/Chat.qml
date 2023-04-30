import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

Item {
    id: root

    implicitWidth: 500
    implicitHeight: 300

    property bool isVisibleChat: true
    signal sendMessage(string type, string message)

    RoundButton {
        width: 30
        height: 30

        text: "+"
        font.bold: true

        background: Rectangle {
            color: "#4f9a94"
            radius: 15
        }

        anchors { top: parent.top; left: parent.left }

        onClicked:  {
            isVisibleChat = isVisibleChat ? false : true
            text = isVisibleChat ? "+" : "-"
        }

        DragHandler { target:root }
    }

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: bar

            spacing: 5
            visible: root.isVisibleChat
            enabled: root.isVisibleChat

            Layout.preferredWidth:  parent.width - 75
            Layout.alignment: Qt.AlignHCenter

            background: Rectangle {
                color: "transparent"
            }

            onCurrentIndexChanged: tfMessage.focus = true

            TabButton {
                text: qsTr("Global")
                font.pointSize: 11
                font.kerning: true
                height: 30

                background: Rectangle {
                    color: bar.currentIndex === 0 ? "orange" : "green"
                    radius: 10
                }

                onClicked: viewChat.model = modelChatGlobal
            }

            TabButton {
                text: qsTr("Table")
                font.pointSize: 11
                font.kerning: true
                height: 30

                background: Rectangle {
                    color: bar.currentIndex === 1 ? "orange" : "green"
                    radius: 10
                }

                onClicked: viewChat.model = modelChatTable
            }

            TabButton {
                text: qsTr("Partner")
                font.pointSize: 11
                font.kerning: true
                height: 30

                background: Rectangle {
                    color: bar.currentIndex === 2 ? "orange" : "green"
                    radius: 10
                }

                onClicked: viewChat.model = modelChatPartner
            }

            TabButton {
                text: qsTr("To Director")
                font.pointSize: 11
                font.kerning: true
                height: 30

                background: Rectangle {
                    color: bar.currentIndex === 3 ? "orange" : "green"
                    radius: 10
                }

                onClicked: viewChat.model = modelChatServer
            }
        }

        Rectangle {
            id: rectMessages

            visible: root.isVisibleChat
            radius: 10
            opacity: 0.3
            layer.enabled: true

            Layout.preferredHeight: 140
            Layout.fillWidth: true

            ScrollView {
                anchors.fill: parent
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                ListView {
                    id: viewChat

                    width: parent.width
                    clip: true
                    model: modelChatGlobal
                    snapMode: ListView.SnapToItem

                    delegate: ItemDelegate {
                        width: viewChat.width
                        height:  txtMessage.height + 3
                        enabled: false

                        Text {
                            id: txtMessage

                            text: "<b>"+sender+"</b>" + ": " + message
                            color: "#424242"
                            font.family: "Ubuntu Light"
                            font.pointSize: 13
                            wrapMode: Text.WrapAnywhere
                            textFormat:Text.RichText
                            font.kerning: true

                            anchors {
                                left: parent.left; leftMargin: 10
                                right: parent.right; rightMargin: 10
                            }
                        }
                    }
                }
            }

            DragHandler { target:root }
        }

        TextField {
            id: tfMessage

            placeholderText: qsTr("Your message")
            visible: root.isVisibleChat

            color: "#424242"
            font.family: "Ubuntu Light"
            font.pointSize: 12
            font.kerning: true

            Layout.preferredHeight: 35
            Layout.preferredWidth: root.width - 95  // - RoundButton send width

            background: Rectangle {
                width: root.width

                color: "white"
                opacity: 0.5
                radius: 10

                RoundButton {
                    text:"send"
                    width: 80
                    height: 28

                    font.family: "Ubuntu Light"
                    font.pointSize: 13
                    font.kerning: true

                    anchors {
                        right: parent.right; rightMargin: 5
                        verticalCenter: parent.verticalCenter
                    }

                    background: Rectangle {
                        color: "#087f23"
                        radius: 10
                    }

                    onClicked: tfMessage.sendMessageToServer()
                }

            }

            onAccepted: tfMessage.sendMessageToServer()

            function sendMessageToServer() {
                var type = "global"

                if (tfMessage.text.trim()) {
                    if(bar.currentIndex === 0) {
                        type = "global"
                    } else if (bar.currentIndex === 1) {
                        type = "table"
                    } else if(bar.currentIndex === 2) {
                        type = "partner"
                    } else {
                        type = "server"
                        root.addMessageSelf(tfMessage.text, "me")
                    }

                    sendMessage(type, tfMessage.text)
                    tfMessage.text=""
                }
            }

            onVisibleChanged: focus = visible ? true : false
        }
    }

    ListModel {
        id: modelChatGlobal
    }

    ListModel {
        id: modelChatTable
    }

    ListModel {
        id: modelChatPartner
    }

    ListModel {
        id: modelChatServer
    }

    Keys.onEscapePressed: root.isVisibleChat = false

    function addMessageGlobal(message, sender) {
        modelChatGlobal.append({"sender": sender, "message": message})
        viewChat.positionViewAtEnd()
    }

    function addMessageTable(message, sender) {
        modelChatTable.append({"sender": sender, "message": message})
        viewChat.positionViewAtEnd()
    }

    function addMessagePartner(message, sender) {
        modelChatPartner.append({"sender": sender, "message": message})
        viewChat.positionViewAtEnd()
    }

    function addMessageSelf(message, sender) {
        modelChatServer.append({"sender": sender, "message": message})
        viewChat.positionViewAtEnd()
    }
}
