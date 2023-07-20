import QtQuick

// Points
Rectangle{
    id: pointrect
    width: 100
    height: 50
    radius: 25
    color: "salmon"
    border.color: "yellow"
    border.width: 4


    property string money: ""


    Behavior on x { SpringAnimation { spring: 2; damping: 0.2 } }
    Behavior on y { SpringAnimation { spring: 25; damping: 0.3; mass: 1.5 } }

    Text {
        id: pointtext
        text: money
        color: "blue"
        font.pointSize: money.length < 7 ? 15 : 13
        font.bold: true
        anchors.centerIn: parent
    }

    Component.onCompleted: {

        pointrect.y = parent.height - pointrect.height - 10


    }
}



