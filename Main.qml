import QtQuick
import QtQuick.Window

import FInitGame 1.0
import FGame 1.0

Window {
    id: root
    width: 440
    height: 680
    visible: true
    title: Qt.application.name
    color: "#263238"

    property bool android: false

    // C++ Classes
    Init{ id: init

        onErrorMessage: (messagetext) => {

            error.errortext = messagetext
            error.open()
        }

        onAppendDownloadedFile: (messagetext) => { downloadmodel.append({"name":messagetext}) }

    }

    Game{ id: game }


    // QML
    FErrorMessage{ id: error  }

    // For downloading game data's
    ListModel{ id: downloadmodel }
    FGameInit{ id: gameinit  }

    // Button to open/close appmenu
    FButton{
        id: menubutton
        height: 45
        width: 45
        imageurl: "qrc:/png/menu.png"
        imagevisible: true
        textvisible: false
        anchors.right: parent.right
        onButtonclicked: {  menu.opening ? menu.close() : menu.open() }
    }

    // Application menu
    FAppMenu{
        id: menu
        onCloseApp: { Qt.quit() }
        onSettingApp: { error.errortext = "Setting is not implemented yet!"; error.open() }
        onPlayApp: {  gamemenu.visible = true }
        onInfoApp: {  error.errortext = "Info is not implemented yet!"; error.open() }
        onUpdateApp: {  error.errortext = "Update is not implemented yet!"; error.open() }
    }

    // Gamemenu
    Rectangle{
        id: gamemenu
        width: parent.width-50
        height: 200
        x:25
        y: root.height/2 - gamemenu.height/2
        color: "transparent"
        visible: false

        FButton{
            id: flagsbutton
            buttontext: qsTr("Flags")
            width: parent.width/2 - 10
            height: parent.height/2 - 15
            x:5; y:5
            disable: game.flags ? false : true
        }
        FButton{
            id: capitalsbutton
            buttontext: qsTr("Capitals")
            width: flagsbutton.width
            height: flagsbutton.height
            anchors.left: flagsbutton.right
            anchors.leftMargin: 5
            anchors.top: flagsbutton.top
            disable: game.capitals ? false : true
        }
        FButton{
            id: bordersbutton
            buttontext: qsTr("Borders")
            width: flagsbutton.width
            height: flagsbutton.height
            anchors.left: flagsbutton.left
            anchors.top: flagsbutton.bottom
            anchors.topMargin: 5
            disable: game.borders ? false : true
        }
        FButton{
            id: areasbutton
            buttontext: qsTr("Areas")
            width: flagsbutton.width
            height: flagsbutton.height
            anchors.left: capitalsbutton.left
            anchors.top: flagsbutton.bottom
            anchors.topMargin: 5
            disable: game.areas ? false : true
        }
    }



    Component.onCompleted: {

        if(Qt.platform.os === "android")
            android = true

        // Check if all neccessery files exist
        init.checkGameData()

        if(init.setup)
            gameinit.visible = true
        else
            gamemenu.visible = true


    }
}
