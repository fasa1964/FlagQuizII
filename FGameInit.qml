import QtQuick

Rectangle{
    id: downloadrect
    width: parent.width-50
    height: parent.height-100
    y:50
    color: "transparent"
    border.color: "white"
    visible: false
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
        color: "#263238"
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
            width: downloadview.width
            height: 50
            border.color: "white"
            color: "transparent"
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
            }
        }
    }
}


