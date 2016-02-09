import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Rectangle {

    id: listmenu
    width: 150
    height: 200
    visible: false
    z: 1000
    clip:false

    border.color: "black"
    border.width: 1

    color: "steelblue"

    signal disappear

    Component.onCompleted: {
        mainWindow.menusDisappear.connect(disappear)
    }

    onDisappear: {
        visible = false
    }
}

