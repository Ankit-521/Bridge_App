import QtQuick 2.12
import Bridge.Constants 1.0
import QtQuick.Controls 2.5
import "../headers"
import "../pages"
import "../controls"
import "../components"
import QtQuick.Layouts 1.3

Rectangle {
    width: parent.width - 16
    height: 56
    radius: 0
    color: "red"
    TextEdit {
        anchors.fill: parent
    }
}
