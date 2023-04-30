pragma Singleton

import QtQuick 2.0

Item {

    readonly property string light: l.name
    readonly property string primary: p.name
    readonly property string secondary: s.name
    readonly property string iconFont: i.name
    readonly property alias colors: mColors

    FontLoader {
        id: l
        source: "../../fonts/HindVadodara-Light.ttf"
    }
    FontLoader {
        id: p
        source: "../../fonts/HindVadodara-Regular.ttf"
    }
    FontLoader {
        id: s
        source: "../../fonts/HindVadodara-Medium.ttf"
    }
    FontLoader {
        id: i
        source: "../../fonts/materialdesignicons-webfont.ttf"
    }

    QtObject {
        id: mColors
        readonly property color primary: "#26282B"
        readonly property color secondary: "#EEEEEE"
        readonly property color primary_light: "#595c5f"
        readonly property color accent: "#1089FF"
        readonly property color white: Qt.rgba(1, 1, 1, 1)
        readonly property color fade: "#E5E5E5"
        readonly property color transparent: "transparent"
        readonly property color placeHolderColor:"#eeeeef"
    }
}
