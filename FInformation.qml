import QtQuick

Rectangle{
    id: inforect

    width: parent.width - 50
    height: parent.height - 100
    color: "#F9FBE7"
    anchors.centerIn: parent
    visible: false

    MouseArea{  anchors.fill: parent }

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
