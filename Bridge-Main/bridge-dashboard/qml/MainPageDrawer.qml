import QtQuick 2.12
import Bridge.Constants 1.0
import QtQuick.Controls 2.5
import "./headers"
import "./components"
import "./controls"

Drawer {
    width: 300

    height: mainPage.height
    modal: false
    y: mainPage.header.toolBar.height
    background: Rectangle {
        color: Constants.colors.primary
    }
    ScrollView {
        anchors.fill: parent

        ListView {
            clip: true
            width: parent.width
            model: ListModel {
                Component.onCompleted: {

                    append({
                               "icon": Icons.mdiAccount,
                               "label": "Profile"
                           })

                    append({
                               "icon": Icons.mdiGamepadVariant,
                               "label": "How to play"
                           })
                    append({
                               "icon": Icons.mdiGamepad,
                               "label": "Game Options"
                           })
                    append({
                               "icon": Icons.mdiArrowExpand,
                               "label": "Full screen"
                           })
                    append({
                               "icon": Icons.mdiForum,
                               "label": "Support"
                           })
                    append({
                               "icon": Icons.mdiHelpCircle,
                               "label": "About"
                           })
                }
            }
            delegate: MouseItem {
                id: mainParent
                width: parent.width
                height: 56

                Row {
                    spacing: 16
                    leftPadding: 12
                    anchors.verticalCenter: parent.verticalCenter
                    IconButton {
                        textItem.text: model.icon
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Text {
                        font.pixelSize: 16
                        color: Constants.colors.white
                        font.family: Constants.secondary
                        anchors.verticalCenter: parent.verticalCenter
                        text: model.label
                    }
                }
            }
        }
    }
}
