import QtQuick

// Errormessage rectangle
Rectangle{
    id: control
    width: parent.width-100
    height: 150
    anchors.centerIn: parent
    color: "transparent"
    border.color: "white"
    border.width: 0.5

    property string errortext: "My errortext"

    FButton{
        id: closebutton
        buttontext: "OK"
        width: 35
        height: 24
        textvisible: true
        textcolor: "white"
        textbold: false

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        onButtonclicked: { control.visible = false }
    }

    Text {
        id: name
        text: errortext
        width: parent.width-20
        x:10; y:10
        color: "white"
        font.letterSpacing: 1.3
        wrapMode: Text.WordWrap
        elide: Text.ElideMiddle
    }

    Rectangle{
        id: errorrect
        anchors.fill: parent
        color: "white"
        opacity: 0.1

    }
}
