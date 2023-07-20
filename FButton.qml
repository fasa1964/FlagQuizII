import QtQuick
import QtQuick.Particles

Rectangle {
    id: control
    implicitHeight: 50
    implicitWidth: 120
    radius: buttonradius
    color: buttoncolor
    border.color: bordercolor

    signal buttonclicked()
    signal finishedTimer()

    function startAnim(){
        animborder.start()
    }

    function startStars(){

        life = 500
        sec = 0
        animtext.start()
        particletimer.start()
        starsparticle.start()
    }


    Timer{
        id: particletimer
        repeat: true
        interval: 50
        onTriggered: {
            life -= 50
            sec += 100
            if(sec >= maxsec){
                particletimer.stop()
                finishedTimer()
            }
        }

    }


    property bool wraptext: false
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
    property int msec: 100

    property int life: 500      // LifeSpan for stars
    property int maxsec: 1000    // LifeSpan for stars
    property int sec: 0

    Text {
        id: tb
        text: buttontext
        width: control.width
        font.pointSize: fontsize
        color: disable ? "gray" : textcolor
        font.letterSpacing: letterspace
        font.bold: disable ? false : textbold
        visible: textvisible
        x: parent.width/2 - tb.width/2
        anchors.verticalCenter: parent.verticalCenter
        wrapMode: Text.WordWrap
        leftPadding: 5
        horizontalAlignment: Text.AlignHCenter
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

    // Animate text
    SequentialAnimation{
        id: animtext
        running: false
        PropertyAnimation{ target: control; property: "fontsize"; to: 20; duration: 50 }
        PropertyAnimation{ target: control; property: "textcolor"; to: "green"; duration: 50 }
        PropertyAnimation{ target: control; property: "fontsize"; to: control.fontsize; duration: 100 }
        PropertyAnimation{ target: control; property: "textcolor"; to: control.textcolor; duration: 100 }
    }


    SequentialAnimation{
        id: animborder
        running: false
        PropertyAnimation{ target: control; property: "bordercolor"; to: "green"; duration: msec }
        PropertyAnimation{ target: control; property: "textcolor"; to: "green"; duration: msec }
        PropertyAnimation{ target: control; property: "buttoncolor"; to: "salmon"; duration: msec }


        PropertyAnimation{ target: control; property: "bordercolor"; to: control.bordercolor ; duration: msec }
        PropertyAnimation{ target: control; property: "textcolor"; to: control.textcolor; duration: msec }

        PropertyAnimation{ target: control; property: "bordercolor"; to: "green"; duration: msec }
        PropertyAnimation{ target: control; property: "textcolor"; to: "green"; duration: msec }

        PropertyAnimation{ target: control; property: "bordercolor"; to: control.bordercolor; duration: msec }
        PropertyAnimation{ target: control; property: "textcolor"; to: control.textcolor; duration: msec }
        PropertyAnimation{ target: control; property: "buttoncolor"; to: control.buttoncolor; duration: msec }

    }


    ParticleSystem {
            id: starsparticle
            anchors.fill: parent
            running: false

            ImageParticle {
                groups: ["stars"]
                anchors.fill: parent
                source: "qrc:///particleresources/star.png"
            }
            Emitter {
                group: "stars"
                emitRate: 100
                lifeSpan: life
                size: 12
                sizeVariation: 8
                anchors.fill: parent
            }

            // ![0]
            ImageParticle {
                anchors.fill: parent
                source: "qrc:///particleresources/star.png"
                alpha: 0
                alphaVariation: 0.2
                colorVariation: 1.0
            }
            // ![0]

            Emitter {
                anchors.centerIn: parent
                emitRate: 10
                lifeSpan: life
                size: 32
                sizeVariation: 8
                velocity: AngleDirection {angleVariation: 180; magnitude: 20}
            }

            Turbulence {
                anchors.fill: parent
                strength: 5
            }
    }

}
