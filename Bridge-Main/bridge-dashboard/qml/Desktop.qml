import QtQuick 2.0
import Bridge.Constants 1.0
import QtQuick.Controls 2.5
import "./headers"
import "./pages"
import "./sections"
import QtQuick.Layouts 1.3

Page {
    id: mainPage
    header: MainPageHeader {}
    Component.onCompleted: {
        forceActiveFocus()
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Escape) {
            console.log("is scale[s]")
            mainWindow.restoreWindow()
        }
    }
    RowLayout {
        anchors.fill: parent
        spacing: 0
        // Constains the drawer
        DrawerContainer {
            id: drawerContainer
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumWidth: 300
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: 300
        }
        Item {
            id: contentContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 300
            Layout.preferredHeight: parent.height
            clip: true
            ContentPage {}
        }
    }
}
