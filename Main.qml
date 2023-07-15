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

    // For downloading game data's
    ListModel{ id: downloadmodel }
    FGameInit{ id: gameinit  }



    Component.onCompleted: {

        if(Qt.platform.os === "android")
            android = true

        // Check if all neccessery files exist
        init.checkGameData()

        if(init.setup)
            gameinit.visible = true

    }
}
