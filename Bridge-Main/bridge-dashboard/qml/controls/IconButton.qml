import QtQuick 2.12
import Bridge.Constants 1.0
import QtQuick.Controls 2.12
import "../components"

// Consists of an icon, MouseItem and a tooltip
// and displays an icon and a tooltip when hovered
MouseItem {
    id: mainParent

    property alias textItem: mtextItem
    property string tip: ""
    width: height
    height: 32
    radius: width / 2

    // icon
    Text {
        opacity: 1
        id: mtextItem
        anchors.centerIn: parent
        font.family: Constants.iconFont
        color: Constants.colors.white
        font.pixelSize: 20
    }

    mousearea.onEntered: {
        if (tip != ""&&parent.enabled)
            toolTip.visible = true
    }
    mousearea.onExited: {
        if (tip != ""&&parent.enabled)
            toolTip.visible = false
    }

    // Displays helper content
    ToolTip {
        id: toolTip
        parent: mainParent
        text: tip
        leftPadding: rightPadding
        rightPadding: 8
        topPadding: bottomPadding
        bottomPadding: 4
        font.family: Constants.secondary
        font.pixelSize: 10
        font.letterSpacing: 0.5
        contentItem: Text {
            text: toolTip.text
            font: toolTip.font
            color: Constants.colors.primary
        }

        background: Rectangle {
            radius: 2
        }
    }
}
