import QtQuick 2.14

Button{
    implicitWidth: bid.width
    implicitHeight: bid.height
    radius: 7

    property alias bidNo: bid.num
    property alias bidSuit: bid.suit
    property alias special: bid.special
    property alias specialText: bid.specialText

    Bid { id: bid; anchors.centerIn: parent }
}
