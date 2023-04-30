import QtQuick 2.12
import Bridge.Constants 1.0
import QtQuick.Controls 2.5
import "../headers"
import "../pages"
import "../controls"
import "../components"
import QtQuick.Layouts 1.3

Rectangle {
    id: drawerContainer
    width: 300
    height: parent.height
    color: Constants.colors.primary
    ScrollView {
        anchors.fill: parent

        ListView {
            clip: true
            width: parent.width
            header: SearchBar {
                x: 8
            }
            model: ListModel {
                Component.onCompleted: {

                    append({
                               "icon": Icons.mdiAccount,
                               "label": "Tournament",
                               "type": ""
                           })

                    append({
                               "icon": Icons.mdiGamepadVariant,
                               "label": "Room",
                               "type": ""
                           })
                    append({
                               "icon": Icons.mdiGamepadVariant,
                               "label": "Chat",
                               "type": ""
                           })
                    append({
                               "icon": Icons.mdiGamepadVariant,
                               "label": "Video",
                               "type": ""
                           })
                    append({
                               "icon": Icons.mdiGamepad,
                               "label": "Game Options",
                               "type": "Controls"
                           })
                    append({
                               "icon": Icons.mdiArrowExpand,
                               "label": "Full screen",
                               "type": "Controls"
                           })
                    append({
                               "icon": Icons.mdiForum,
                               "label": "Support",
                               "type": "Controls"
                           })
                    append({
                               "icon": Icons.mdiHelpCircle,
                               "label": "About",
                               "type": "Controls"
                           })
                }
            }
            section.property: "type"
            section.criteria: ViewSection.FullString
            section.delegate: Item {
                property alias sectionText: inner_sectionText
                width: parent.width
                height: dp(48)
                Separator {

                    y: dp(8)
                }
                Body1 {
                    id: inner_sectionText

                    font.pixelSize: sp(10)
                    font.bold: true
                    font.letterSpacing: 1.7
                    font.capitalization: Font.AllUppercase
                    text: section
                    leftPadding: dp(20)
                    y: parent.height - height
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
