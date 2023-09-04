import QtQuick
import QtQuick.Controls

Rectangle {
    id: settingsrect
    width: parent.width - 50
    height: parent.height - 100
    color: "white"
    anchors.centerIn: parent
    z:1
    visible: false

    property alias langcurrentindex: languageBox.currentIndex

    MouseArea{ anchors.fill: parent }

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
