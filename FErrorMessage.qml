import QtQuick

// Errormessage rectangle
Rectangle{
    id: control
    width: parent.width-100
    height: 150
    y: root.height/2 - control.height/2
    x: root.width
    color: "transparent" //"#263238"
    border.color: "white"
    border.width: 0.5
    z:3

    MouseArea{ anchors.fill: parent  }

    property string errortext: "My errortext"


    function open(){ openanim.start() }
    function close(){ closeanim.start() }


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
        onButtonclicked: { close() }
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
        opacity: 0.5

    }

    NumberAnimation on x {
        id: openanim
        to: root.width/2 - control.width/2
        from: root.width
        duration: 500
        running: false
        easing.type: Easing.OutExpo


    }

    NumberAnimation on x {
        id: closeanim
        to: root.width
        from: 0
        duration: 500
        running: false
        easing.type: Easing.OutExpo

    }
}
