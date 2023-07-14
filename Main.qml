import QtQuick
import QtQuick.Window

import FInitGame 1.0

Window {
    id: root
    width: 440
    height: 680
    visible: true
    title: Qt.application.name
    color: "#263238"

    property bool android: false

    Init{ id: init

        onErrorMessage: (messagetext) => {

            error.errortext = messagetext
            error.visible = true
        }

        onAppendDownloadedFile: (messagetext) => { downloadmodel.append({"name":messagetext}) }

    }

    FErrorMessage{ id: error; visible: false  }

    ListModel{ id: downloadmodel }

    Rectangle{
        id: downloadrect
        width: parent.width-50
        height: parent.height-100
        y:50
        color: "transparent"
        border.color: "white"
        visible: true
        anchors.centerIn: parent

        FButton{
            id: closebutton
            imageurl: "qrc:/png/close.png"
            imagevisible: true
            textvisible: false
            width: 24
            height: 24
            x:5; y:5
            z:2
            onButtonclicked: {

                    downloadrect.visible = false
                    init.cancelDownloading()

            }
        }

        FButton{
            id: downloadbutton
            width: 220
            textcolor: "white"
            textbold: false
            buttontext: "Download game datas!"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 10; z:2
            onButtonclicked: { init.startDownloadGameDatas(); downloadbutton.visible = false }
        }


        ListView{
            id: downloadview
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            model: downloadmodel
            spacing: 5
            clip: true

            header: Rectangle{
                    id: headerrect
                    width: parent.width-100
                    height: grid.height + 10
                    x:50
                    color: "transparent"
                    border.color: "white"
                    visible: init.setup ? true : false

                    Grid{
                        id: grid
                        columns: 2
                        rows: 3
                        columnSpacing: 10
                        rowSpacing: 5
                        x:5; y:5
                        Text {
                            text:  qsTr("File:")
                            color: "white"
                        }
                        Text {
                            text:  init.currentFile
                            color: "white"
                            font.letterSpacing: 2
                            font.pointSize: 10
                            font.bold: true
                        }
                        Text {
                            text:  qsTr("Kb:")
                            color: "white"
                        }
                        Text {
                            text:  init.kilobytes
                            color: "white"
                            font.letterSpacing: 2
                            font.pointSize: 10
                            font.bold: true
                        }
                        Text {
                            text:  qsTr("Flags:")
                            color: "white"
                        }
                        Text {
                            text:  init.flagsCount
                            color: "white"
                            font.letterSpacing: 2
                            font.pointSize: 10
                            font.bold: true
                        }
                    }
            }

            delegate: Rectangle{
                id: delegaterect
                width: downloadview.width-50
                height: 50
                border.color: "white"
                color: "transparent"
                anchors.horizontalCenter: parent.horizontalCenter
                Row{
                    id: row
                    width: parent.width
                    spacing: 5
                    anchors.verticalCenter: parent.verticalCenter
                    x:15
                    Text {
                        text: name
                        color: "white"
                    }
//                    Text {
//                        text: init.currentFile === name ? init.progress : 0
//                        color: "white"
//                    }

//                    Rectangle{
//                        id: okrect
//                        width: 28
//                        height: 28
//                        color: "transparent"
//                        visible: init.currentFile === name && init.fileDownloaded ? true : false

//                        Image {
//                            anchors.fill: parent
//                            source: "qrc:/png/ok.png"
//                        }
//                    }
                }

            }
        }
    }   



    Component.onCompleted: {

        if(Qt.platform.os === "android")
            android = true

        // Check if all neccessery files exist
        init.checkGameData()

    }
}
