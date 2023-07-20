import QtQuick
import QtQuick.Particles
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

    function showGameMenu(){
        gamefield.visible = false
        gamemenu.visible = true
    }

    function gameIsOver(text){
        // Do something when game is over
        //console.log("Game over:" + text)
    }


    function createPointObject(text){
            var component = Qt.createComponent("FPoints.qml");
                if (component.status === Component.Ready)
                {
                    var point = component.createObject(gamefield, {x: 40, y: 0});
                    if(point === null){
                        showMessage("Error", "Creating object's failed!", 0, true)
                    }else{
                        point.money = text
                        var xpos = pointsModel.count * 15
                        point.x = gamefield.x + 10 + xpos

                        pointsModel.append({"obj":point})
                    }
                }
                else
                {
                    showMessage("Error", "Creating object's failed!", 0, true)
                }
    }
    ListModel{ id: pointsModel }

    // C++ Classes
    Init{ id: init

        onErrorMessage: (messagetext) => {

            error.errortext = messagetext
            error.open()
        }

        onAppendDownloadedFile: (messagetext) => { downloadmodel.append({"name":messagetext}) }

    }

    Game{ id: game
        onErrorMessage: (errortext) => {  error.errortext = errortext; error.open()  }
        onSelectButton: (button) => {

                            if(button === "A")
                                buttonA.startAnim();
                            if(button === "B")
                                buttonB.startAnim();
                            if(button === "C")
                                buttonC.startAnim();
                            if(button === "D")
                                buttonD.startAnim();
                        }
        onCredits: (euro) => { createPointObject(euro) }
        onGameOver: (euro) => { gameIsOver(euro) }
    }


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
        onPlayApp: {  showGameMenu() }
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
            disable: game.flagsAvailable ? false : true
            onButtonclicked: { game.startFlagsGame(); gamemenu.visible = false; gamefield.visible = true }
        }
        FButton{
            id: capitalsbutton
            buttontext: qsTr("Capitals")
            width: flagsbutton.width
            height: flagsbutton.height
            anchors.left: flagsbutton.right
            anchors.leftMargin: 5
            anchors.top: flagsbutton.top
            disable: game.capitalsAvailable ? false : true
            onButtonclicked: game.startCapitalsGame()
        }
        FButton{
            id: bordersbutton
            buttontext: qsTr("Borders")
            width: flagsbutton.width
            height: flagsbutton.height
            anchors.left: flagsbutton.left
            anchors.top: flagsbutton.bottom
            anchors.topMargin: 5
            disable: game.bordersAvailable ? false : true
            onButtonclicked: game.startBordersGame()
        }
        FButton{
            id: areasbutton
            buttontext: qsTr("Areas")
            width: flagsbutton.width
            height: flagsbutton.height
            anchors.left: capitalsbutton.left
            anchors.top: flagsbutton.bottom
            anchors.topMargin: 5
            disable: game.areasAvailable ? false : true
        }
    }

    // Gamefield
    Rectangle{
        id: gamefield
        width: parent.width
        height: parent.height - menu.height
        y: menu.height
        color: "transparent"
        visible: false

        Text {
            text: game.questionCounter
            color: "white"
            anchors.right: questionrect.right
            anchors.bottom: questionrect.top
        }

        FButton{
            id: cancelbutton
            width: 15
            height: 15
            imageurl: "qrc:/png/close.png"
            imagevisible: true
            textvisible: false
            anchors.left: questionrect.left
            anchors.top: gamefield.top
            anchors.bottomMargin: 5
            onButtonclicked: {   game.cancelGame(); showGameMenu()  }
        }

        // Joker
        Row{
            id: jokerrow
            width: questionrect.width
            height: 50
            anchors.left: questionrect.left
            anchors.bottom: questionrect.top
            spacing: 10
            FButton{
                id: joker50Button
                width: 50
                height: jokerrow.height
                bordercolor: "transparent"
                imageurl: "qrc:/png/Joker50.png"
                imagevisible: true
                textvisible: false
                onButtonclicked: { game.setJoker50(); joker50Button.disable = true }
            }
            FButton{
                id: jokerTelButton
                width: 50
                height: jokerrow.height
                bordercolor: "transparent"
                imageurl: "qrc:/png/JokerTel.png"
                imagevisible: true
                textvisible: false
                onButtonclicked: { game.setJokerTel(); jokerTelButton.disable = true }
            }
            FButton{
                id: jokerPubButton
                width: 50
                height: jokerrow.height
                bordercolor: "transparent"
                imageurl: "qrc:/png/JokerPub.png"
                imagevisible: true
                textvisible: false
                onButtonclicked: { game.setJokerPub(); jokerPubButton.disable = true }
            }
        }

        Rectangle{
            id: questionrect
            width: parent.width-30
            height: 180
            border.color: "lightblue"
            x:15; y:100


            Rectangle{
                id: imagerect
                width: questionrect.width-10
                height: questionrect.height-10
                x:5; y:5
                Image {
                    id: qimage
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    source: "file:/" + game.flagPath
                }
            }
        }

        // Buttons
        FButton{
            id: buttonA
            buttontext: game.answerA
            width: questionrect.width/2 - 5
            height: 100
            textcolor: "white"
            anchors.top: questionrect.bottom
            anchors.topMargin: 10
            anchors.left: questionrect.left
            onButtonclicked: {

                if(game.solution === buttonA.buttontext)
                    buttonA.startStars()
                else
                    game.startNextQuestion()

                game.setAnswer(buttonA.buttontext)

            }
            onFinishedTimer: {  game.startNextQuestion()  }

        }
        FButton{
            id: buttonB
            buttontext: game.answerB
            width: buttonA.width
            height: buttonA.height
            textcolor: "white"
            anchors.right: questionrect.right
            anchors.top: buttonA.top
            onButtonclicked: {

                if(game.solution === buttonB.buttontext)
                    buttonB.startStars()
                else
                    game.startNextQuestion()

                game.setAnswer(buttonB.buttontext)

            }
            onFinishedTimer: {  game.startNextQuestion()  }

        }
        FButton{
            id: buttonC
            buttontext: game.answerC
            width: buttonA.width
            height: buttonA.height
            textcolor: "white"
            anchors.left: buttonA.left
            anchors.top: buttonA.bottom
            anchors.topMargin: 10
            onButtonclicked: {

                if(game.solution === buttonC.buttontext)
                    buttonC.startStars()
                else
                    game.startNextQuestion()

                game.setAnswer(buttonC.buttontext)

            }
            onFinishedTimer: {  game.startNextQuestion()  }

        }
        FButton{
            id: buttonD
            buttontext: game.answerD
            width: buttonA.width
            height: buttonA.height
            textcolor: "white"
            anchors.left: buttonB.left
            anchors.top: buttonA.bottom
            anchors.topMargin: 10
            onButtonclicked: {

                if(game.solution === buttonD.buttontext)
                    buttonD.startStars()
                else
                    game.startNextQuestion()

                game.setAnswer(buttonD.buttontext)
                //game.startNextQuestion()
            }
            onFinishedTimer: { game.startNextQuestion()  }
        }
    }

    ParticleSystem {
            anchors.fill: parent
            running: false
            ImageParticle {
                groups: ["stars"]
                anchors.fill: parent
                source: "qrc:///particleresources/star.png"
            }
            Emitter {
                group: "stars"
                emitRate: 800
                lifeSpan: 2400
                size: 24
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
                emitRate: 400
                lifeSpan: 2400
                size: 48
                sizeVariation: 8
                velocity: AngleDirection {angleVariation: 180; magnitude: 60}
            }

            Turbulence {
                anchors.fill: parent
                strength: 2
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
            showGameMenu()


    }
}
