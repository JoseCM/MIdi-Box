import QtQuick 2.0

Rectangle {

    anchors.centerIn: parent
    width: 100
    height: 22
    color: "white"
    clip: true
    border.color: "black"
    border.width: 1


    property TextInput input: textin

    TextInput{
        anchors.fill: parent
        anchors.margins: 1
        id: textin
        cursorVisible: true
    }

}
