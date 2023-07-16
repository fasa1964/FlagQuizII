import QtQuick

// Application menu
Rectangle{
    id: appmenurect
    height: 45
    width: parent.width
    color: "transparent"
    border.color: "white"
    x: parent.width
    visible: true

    property bool opening: false

    function open(){ openanim.start() }
    function close(){ closeanim.start() }

    signal closeApp()
    signal infoApp()
    signal updateApp()
    signal settingApp()
    signal playApp()

    Row{
        width: parent.width
        height: parent.height-8
        anchors.verticalCenter: parent.verticalCenter
        spacing: 5
        x:5
        FButton{
            id: quitbutton
            width: 45
            height: parent.height
            imageurl: "qrc:/png/quit.png"
            imagevisible: true
            textvisible: false
            onButtonclicked: {  closeApp() }
        }
        FButton{
            id: infobutton
            width: 45
            height: parent.height
            imageurl: "qrc:/png/info.png"
            imagevisible: true
            textvisible: false
            onButtonclicked: {  infoApp() }
        }
        FButton{
            id: updatebutton
            width: 45
            height: parent.height
            imageurl: "qrc:/png/update.png"
            imagevisible: true
            textvisible: false
            onButtonclicked: {  updateApp() }
        }
        FButton{
            id: settingsbutton
            width: 45
            height: parent.height
            imageurl: "qrc:/png/settings.png"
            imagevisible: true
            textvisible: false
            onButtonclicked: {  settingApp() }
        }
        FButton{
            id: playbutton
            width: 45
            height: parent.height
            imageurl: "qrc:/png/play.png"
            imagevisible: true
            textvisible: false
            onButtonclicked: {  playApp() }
        }
    }



    NumberAnimation on x {
        id: openanim
        to: 0
        from: root.width
        duration: 500
        running: false
        easing.type: Easing.OutExpo
        onFinished: {  opening = true }

    }

    NumberAnimation on x {
        id: closeanim
        to: root.width
        from: 0
        duration: 500
        running: false
        easing.type: Easing.OutExpo
        onFinished: {  opening = false  }
    }

}
