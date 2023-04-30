import QtQuick 2.12
import Bridge.Constants 1.0
import QtQuick.Controls 2.5
import "../controls"

Rectangle {
    property alias toolBar: mToolBar
    width: parent.width
    height: mCol.height
    color: Constants.colors.primary

    Column {
        id: mCol
        width: parent.width
        spacing: 0
        ToolBar {
            id: mToolBar
            width: parent.width
            height: 50
            background: Rectangle {
                color: Constants.colors.transparent
            }
            ToolButton {
                height: 48
                width: mRow.width
                x: 16

                background: Rectangle {
                    color: Constants.colors.transparent
                }
                onClicked: {
                    console.log("is ipemnd")
                    drawer.open()
                }
                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    id: mRow
                    spacing: 8
                    rightPadding: 16
                    Item {
                        anchors.verticalCenter: parent.verticalCenter
                        width: height
                        height: 24
                        Image {
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit
                            smooth: true
                            mipmap: true
                            source: "../../../common_assets/images/menu_white.png"
                        }
                    }
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 14
                        color: Constants.colors.white
                        text: "Menu"
                        font.capitalization: Font.AllUppercase
                        font.letterSpacing: 0.3
                        font.family: Constants.secondary
                    }
                }
            }
            Row {
                x: parent.width - (width + 16)
                anchors.verticalCenter: parent.verticalCenter
                spacing: 16

                // more icons
                Repeater {
                    model: ListModel {
                        Component.onCompleted: {
                            append({
                                       "icon": Icons.mdiSettings,
                                       "tip": "Settings"
                                   })
                            append({
                                       "icon": Icons.mdiArrowExpand,
                                       "tip": "Full screen"
                                   })
                        }
                    }
                    delegate: IconButton {
                        textItem.text: model.icon
                        tip: model.tip
                        mousearea.onClicked: {
                            if (index == 1)
                                mainWindow.toggleFullScreen()
                        }
                    }
                }
                // User image
                IconButton {
                    anchors.verticalCenter: parent.verticalCenter
                    textItem.text: Icons.mdiAccount
                }
            }
            Text {
                anchors.centerIn: parent
                font.pixelSize: 24
                color: Constants.colors.white
                font.family: Constants.secondary
                text: qsTr("Bridge Club")
            }
        }
        Divider {
            color: "#0e0f10"
        }
      }

    // Divider
    Divider {
        y: parent.height - height
    }
}
