import QtQuick

Rectangle {
    id: control
    implicitHeight: 50
    implicitWidth: 120
    radius: buttonradius
    color: buttoncolor
    border.color: bordercolor

    signal buttonclicked()

    property bool disable: false
    property bool imagevisible: false
    property bool textvisible: true
    property bool textbold: true
    property string buttontext: "This is my Button"
    property string textcolor: "#1e5aad"
    property string buttoncolor: "transparent"
    property string bordercolor: "gray"
    property url imageurl: ""
    property real letterspace: 2
    property real buttonradius: 5
    property real imageopacity: 1.0

    property real fontsize: root.android ? 13 : 12

    Text {
        id: tb
        text: buttontext
        font.pointSize: fontsize
        color: disable ? "gray" : textcolor
        font.letterSpacing: letterspace
        font.bold: disable ? false : textbold
        visible: textvisible
        x: parent.width/2 - tb.width/2
        anchors.verticalCenter: parent.verticalCenter

    }

    Image {
        id: image
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        smooth: true
        visible: imagevisible
        source: imageurl
        opacity: disable ? 0.25 : imageopacity
    }


    MouseArea{
        anchors.fill: parent
        onPressed: {  if(!disable){ anim.running = true }  }
        onReleased: { if(!disable){ anim2.running = true;  buttonclicked() } }
    }


    ColorAnimation on color {
        id: anim
        from: buttoncolor
        to: "gray"
        duration: 100
        running: false
    }
    ColorAnimation on color {
        id: anim2
        from: "gray"
        to: buttoncolor
        duration: 100
        running: false
    }

}
