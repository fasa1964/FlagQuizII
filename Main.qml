import QtQuick
import QtCore
import QtQuick.Particles
import QtQuick.Window
import QtQuick.Controls
import QtMultimedia

import FInitGame 1.0
import FGame 1.0
import FGameSettings 1.0


Window {
    id: root
    width: 440
    height: 680
    visible: true
    title: Qt.application.name
    color: "#263238"

    property bool android: false

    // For timerrectangle
    property int delay: 20
    property int wfill: 0
    property int msecond: 0
    property int second: 0

    // Volume of sound
    property real soundvolume: 0.5
    property bool playsound: true


    function showGameMenu(){

        gamefield.visible = false
        gamemenu.visible = true


        gameovertext.text = ""
        gameovertext.visible = false

        // When starting new game don't stop thous timers
        //gameover_timer.running = false
        //particleGameOver.running = false

        joker50Button.disable = false
        jokerPubButton.disable = false
        jokerTelButton.disable = false
    }

    function removePoints(){

        while(pointsModel.count > 0){
            pointsModel.get(0).obj.destroy()
            pointsModel.remove(0)
        }


    }

    function gameIsOver(text){

        // Do something when game is over
        questiontimer.stop()
        gamefield.visible = false

        gameovertext.visible = true
        gameovertext.text = text

        overlife = 2400
        msecond = 0
        second = 0

        if(text.length > 0){
            gameover_timer.start()
            particleGameOver.start()
        }

        if(text === "1.000.000€")
            applause.play()

        removePoints()

        console.log(text)
    }

    function nextQuestion(){
        msecond = 0
        second = 0
        wfill = 0
        questiontimer.start()
        game.startNextQuestion()
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

    // Timer for question
    Timer{
        id: questiontimer
        repeat: true
        interval: 100
        onTriggered: { timeout() }
    }

    function timeout(){

        msecond += 100
        wfill += 10

        if(msecond >= 1000){ // one second

            msecond = 0
            second++
        }

        if(second >= delay)
            nextQuestion()
    }

    function readSettingsVariables(){

        if(settings.enableLanguage){

            if(languageModel.count <= 0){

                var list = settings.getAvailableLanguages()
                for(var i = 0; i < list.length; i++)
                    languageModel.append({"name":list[i]})

                languageBox.currentIndex = settings.currentIndex
            }

        }

    }

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

    GameSettings{
        id: settings
        onErrorOccurred: (errortext) => {  error.errortext = errortext; error.open() }
    }

    // QML
    FErrorMessage{ id: error  }

    // Sounds
    SoundEffect{
        id: dump
        source: Qt.resolvedUrl( "qrc:/wav/beep.wav" )
        volume: soundvolume
    }
    SoundEffect{
        id: slash
        source: Qt.resolvedUrl( "qrc:/wav/slash.wav" )
        volume: soundvolume
    }
    SoundEffect{
        id: applause
        source: Qt.resolvedUrl( "qrc:/wav/applause.wav" )
        volume: soundvolume
    }

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
        onSettingApp: { settingsrect.visible = true; readSettingsVariables() }
        onPlayApp: {  showGameMenu() }
        onInfoApp: {  inforect.visible = true }
        onUpdateApp: {  error.errortext = "Update is not implemented yet!"; error.open() }
    }

    // Appicon
    Image {
        id: appicon
        width: parent.width/3
        fillMode: Image.PreserveAspectFit
        source: "qrc:/png/flagsicon.png"
        x: parent.width/2 - appicon.width/2
        y:50
        visible: gamemenu.visible ? true : false
    }


    // Gamemenu
    Rectangle{
        id: gamemenu
        width: parent.width-50
        height: 200
        x:25
        y: root.height/2 - gamemenu.height/2 - 20
        color: "transparent"
        visible: false

        Text {

            text: qsTr("Language: ") +  settings.languageName
            color: "white"
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.bottom: flagsbutton.top
            anchors.bottomMargin: 10
        }

        FButton{
            id: flagsbutton
            buttontext: qsTr("Flags")
            width: parent.width/2 - 10
            height: parent.height/2 - 15
            x:5; y:5
            disable: game.flagsAvailable ? false : true
            onButtonclicked: { game.startFlagsGame(); nextQuestion() ; gamemenu.visible = false; gamefield.visible = true }
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
            onButtonclicked: { game.startCapitalsGame(); nextQuestion() ; gamemenu.visible = false; gamefield.visible = true}
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
            onButtonclicked: { game.startBordersGame(); nextQuestion() ; gamemenu.visible = false; gamefield.visible = true}
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
            onButtonclicked: { game.startAreasGame(); nextQuestion() ; gamemenu.visible = false; gamefield.visible = true}
        }
        FButton{
            id: continentbutton
            buttontext: qsTr("Continent")
            width: flagsbutton.width
            height: flagsbutton.height
            anchors.left: bordersbutton.left
            anchors.top: bordersbutton.bottom
            anchors.topMargin: 5
            disable: game.continentAvailable ? false : true
            onButtonclicked: { game.startContinentGame(); nextQuestion() ; gamemenu.visible = false; gamefield.visible = true }
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
                    opacity: game.flags ? 1.0 : 0.2
                }
            }

            Text {
                id: question
                text: game.question
                anchors.centerIn: parent
                color: "#0040FF"
                font.pointSize: android ? 12 : 15
                visible: game.flags ? false : true
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

                if(game.solution === buttonA.buttontext){
                    buttonA.startStars()
                    if(playsound)
                        slash.play()
                }else{
                    nextQuestion()
                    if(playsound)
                        dump.play()
                }

                game.setAnswer(buttonA.buttontext)


            }
            onFinishedTimer: { nextQuestion() /*game.startNextQuestion()*/  }

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

                if(game.solution === buttonB.buttontext){
                    buttonB.startStars()
                    if(playsound)
                        slash.play()

                }else{
                    nextQuestion()
                    if(playsound)
                        dump.play()
                }

                game.setAnswer(buttonB.buttontext)

            }
            onFinishedTimer: { nextQuestion() }

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

                if(game.solution === buttonC.buttontext){
                    buttonC.startStars()
                    if(playsound)
                        slash.play()

                }else{
                    nextQuestion()
                    if(playsound)
                        dump.play()
                }

                game.setAnswer(buttonC.buttontext)

            }
            onFinishedTimer: {  nextQuestion()  }

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

                if(game.solution === buttonD.buttontext){
                    buttonD.startStars()
                    if(playsound)
                        slash.play()
                }else{
                    nextQuestion()
                    if(playsound)
                        dump.play()
                }

                game.setAnswer(buttonD.buttontext)

            }
            onFinishedTimer: { nextQuestion()  }
        }


        Rectangle{
            id: timerrectangle
            width: buttonA.width * 2 + 10
            height: 30
            color: "transparent"
            border.color: "white"
            border.width: 0.5
            anchors.left: buttonC.left
            anchors.top: buttonC.bottom
            anchors.topMargin: 5


            Rectangle{
                id: fillrect
                height: timerrectangle.height - 2
                width:  timerrectangle.width/(delay*100) * wfill - 2
                color: "steelblue"
                y:1; x:1
            }

            Text{ text: second; anchors.centerIn: parent; color: "white" }
        }
    }

    // Game Over
    ParticleSystem {
            id: particleGameOver
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
                lifeSpan: overlife
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
                lifeSpan: overlife
                size: 48
                sizeVariation: 8
                velocity: AngleDirection {angleVariation: 180; magnitude: 60}
            }

            Turbulence {
                anchors.fill: parent
                strength: 2
            }
    }

    property int overlife: 2400

    Text {
            id: gameovertext
            text: ""
            anchors.centerIn: parent
            font.pointSize: 48
            color: "blue"
            style: Text.Sunken;
            styleColor: "#AAAAAA"
    }


    Timer{
        id: gameover_timer
        repeat: true
        interval: 100
        onTriggered: {

            msecond += 100
            if(msecond >= 1000){ // one second
                msecond = 0
                second++
            }

            overlife = overlife - 75

            if(overlife < 0){

                particleGameOver.stop()
                gameover_timer.stop()
            }

        }
    }

    // Settings
    ListModel{ id: languageModel }
    Rectangle {
        id: settingsrect
        width: parent.width - 50
        height: parent.height - 100
        color: "white"
        anchors.centerIn: parent
        z:1
        visible: false

        FButton{
            id: cb
            width: 18
            height: 18
            x:5; y:5
            imageurl: "qrc:/png/close.png"
            imagevisible: true
            textvisible: false
            buttoncolor: "gray"
            bordercolor: "gray"
            onButtonclicked: { settingsrect.visible = false }
        }


        Grid{
            id: sgrid
            width: parent.width
            columns: 2
            columnSpacing: 10
            rows: 4
            rowSpacing: 10

            x:5; y:30

            Text {
                text: "Sound:"
                font.pointSize: 12
                color: "gray"
            }

            CheckBox{
                id: sbox
                width: 20
                height: 20
                checked: playsound
                onClicked: { playsound = sbox.checked }
            }

            Text {
                id: vt
                text: "Volume:"
                font.pointSize: 12
                color: "gray"
            }

            Slider{
               id: volumeslider
               width: parent.width - vt.width - 50
               from: 0; to: 10
               stepSize: 0.1
               value: root.soundvolume * 10
               snapMode: Slider.SnapAlways
               onValueChanged: { root.soundvolume = (volumeslider.value/10).toFixed(2) }

               Text {
                   id: vd
                   text: volumeslider.value.toFixed(2)
                   anchors.centerIn: parent
                   font.pointSize: 12
                   color: "blue"
               }

            }

            Text {
                id: lt
                text: "Language:"
                font.pointSize: 12
                color: "gray"
            }

            ComboBox{
                id: languageBox
                width: parent.width - lt.width - 30
                model: languageModel
                enabled: settings.enableLanguage
                onCurrentTextChanged: { settings.setLanguage(currentText); game.setLanguageCode( settings.languageCode ) }

            }
        }
    }

    // Information
    Rectangle{
        id: inforect

        width: parent.width - 50
        height: parent.height - 100
        color: "#F9FBE7"
        anchors.centerIn: parent
        visible: false


        FButton{
            id: cbi
            width: 18
            height: 18
            x:5; y:5
            imageurl: "qrc:/png/close.png"
            imagevisible: true
            textvisible: false
            buttoncolor: "gray"
            bordercolor: "gray"
            onButtonclicked: { inforect.visible = false }
        }

        Image {
            anchors.centerIn: parent
            source: "qrc:/png/flagsicon.png"
            opacity: 0.10
        }

        Text {
            id: infocaption
            text: qsTr("About ") + Qt.application.name
            font.bold: true
            font.pointSize: root.android ? 13 : 14
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#1565C0"
            y:10
        }

        Grid{
            id: infogrid
            width: inforect.width - 20
            anchors.top: infocaption.bottom
            anchors.topMargin: 10
            columns: 2
            columnSpacing: 10
            rows: 9
            rowSpacing: 6
            leftPadding: 10


            Text {
                text: qsTr("Name:")
                font.pointSize: root.android ? 13 : 12
                color: "#455A64"
            }

            Text {
                text: Qt.application.name
                font.pointSize: root.android ? 13 : 12
                color: "#1565C0"
                font.letterSpacing: 1.5
            }
            Text {
                text: qsTr("Version:")
                font.pointSize: root.android ? 13 : 12
                color: "#455A64"
            }

            Text {
                text: Qt.application.version
                font.pointSize: root.android ? 13 : 12
                color: "#1565C0"
                font.letterSpacing: 1.5
            }
            Text {
                text: qsTr("IDE:")
                font.pointSize: root.android ? 13 : 12
                color: "#455A64"
            }

            Text {
                text: "Qt6 QML C++"
                font.pointSize: root.android ? 13 : 12
                color: "#1565C0"
                font.letterSpacing: 1.5
            }
            Text {
                text: qsTr("Licence:")
                font.pointSize: root.android ? 13 : 12
                color: "#455A64"
            }

            Text {
                text: "GPL v. 3.0"
                font.pointSize: root.android ? 13 : 12
                color: "#1565C0"
                font.letterSpacing: 1.5
            }
            Text {
                text: qsTr("Developed:")
                font.pointSize: root.android ? 13 : 12
                color: "#455A64"
            }

            Text {
                text: "Farschad Saberi"
                font.pointSize: root.android ? 13 : 12
                color: "#1565C0"
                font.letterSpacing: 1.5
            }
            Text {
                id: dt
                text: qsTr("Description:")
                font.pointSize: root.android ? 13 : 12
                color: "#455A64"
            }

            Text {
                id: descriptiontext
                width: parent.width - dt.width - 15
                text: game.getDescriptionText()
                font.pointSize: root.android ? 13 : 12
                color: "#1565C0"
                font.letterSpacing: 1.2
                wrapMode: Text.WordWrap
            }

            Text {
                text: qsTr("GPLv3:")
                font.pointSize: root.android ? 13 : 12
                color: "#455A64"
            }

            Text {
                text: "Info about GPLv3"
                font.pointSize: root.android ? 13 : 12
                color: "#1565C0"
                font.letterSpacing: 1.5
                MouseArea{
                    anchors.fill: parent
                    onClicked: { Qt.openUrlExternally("https://www.gnu.org/licenses/gpl-3.0.en.html")       }
                }
            }

            Text {
                text: qsTr("IDE Qt 6:")
                font.pointSize: root.android ? 13 : 12
                color: "#455A64"
            }

            Text {
                text: "Info about Qt 6"
                font.pointSize: root.android ? 13 : 12
                color: "#1565C0"
                font.letterSpacing: 1.5
                MouseArea{
                    anchors.fill: parent
                    onClicked: { Qt.openUrlExternally("https://www.qt.io/product/development-tools")       }
                }
            }
            Text {
                text: qsTr("Source-Code:")
                font.pointSize: root.android ? 13 : 12
                color: "#455A64"
            }

            Text {
                text: "On GitHub"
                font.pointSize: root.android ? 13 : 12
                color: "#1565C0"
                font.letterSpacing: 1.5
                MouseArea{
                    anchors.fill: parent
                    onClicked: { Qt.openUrlExternally("https://github.com/fasa1964/FlagQuizII") }
                }
            }

        }

    }


    // Values to store
    Settings{
        id: appsettings
        property alias sv: root.soundvolume
        property alias ps: root.playsound
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
