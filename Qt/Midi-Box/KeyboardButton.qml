import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle {

    color: "steelblue"
    property string letter

    height: 35
    width: 35
    radius: 5
    border.width: 1
    border.color: "black"

    signal pressed()

    onPressed: {
        console.log("pressed")
        keyboard.insert(letter)
    }

    Text {
        text: letter
        anchors.centerIn: parent
    }

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
        onClicked: parent.pressed()
    }

}
