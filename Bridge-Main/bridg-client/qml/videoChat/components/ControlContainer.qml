import QtQuick 2.11
import QtQuick.Controls 2.12

Item {
    id: controlContainer
    width: mRow.width
    height: mRow.height
    anchors.right: parent.right
    anchors.rightMargin: 16

    Row {
        id: mRow
        spacing: 16
        rightPadding: 8
        Repeater {
            model: ListModel {
                Component.onCompleted: {
                    append({
                               "icon": "../../images/maximize.png"
                           })

                    append({
                               "icon": "../../images/close.png"
                           })
                }
            }
            delegate: Item {
                width: height
                height: 20

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        switch (index) {
                        case 1:
                            toggle()
                            break
                        case 0:
                            expanded ? reset() : expand()
                            break
                        }
                    }
                }
                Image {
                    opacity: 0.8
                    anchors.fill: parent
                    source: model.icon
                }
            }
        }
    }
}
