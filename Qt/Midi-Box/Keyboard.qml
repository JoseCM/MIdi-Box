import QtQuick 2.3

import QtQuick.Controls 1.0
import QtQuick.Layouts 1.1

Rectangle {

    id: keyboard
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    height: keyboardLayout.height + 20
    width: parent.width
    color: "grey"
    border.width: 1
    border.color: "black"

    property bool caps: false
    property MyTextInput input

    function insert(str){

        if(!caps)
            str = str.toLowerCase()

        input.input.insert(input.input.length, str)
    }

    function remove(n){
        input.input.remove(input.input.length - n, input.input.length)
    }

    ColumnLayout {

        id: keyboardLayout
        anchors.right: parent.right
        anchors.left: parent.left
        //anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 5
        spacing: 2

        RowLayout {

            spacing: 2
            Layout.alignment: Qt.AlignCenter

            KeyboardButton {letter: "Q"}
            KeyboardButton {letter: "W"}
            KeyboardButton {letter: "E"}
            KeyboardButton {letter: "R"}
            KeyboardButton {letter: "T"}
            KeyboardButton {letter: "Y"}
            KeyboardButton {letter: "U"}
            KeyboardButton {letter: "I"}
            KeyboardButton {letter: "O"}
            KeyboardButton {letter: "P"}
            KeyboardButton {
                letter: "<-"
                width: 70
                onPressed: {
                    console.log("remove")
                    input.input.remove(input.input.length - 3, input.input.length)
                }
            }

        }

        RowLayout {

            spacing: 2
            Layout.alignment: Qt.AlignCenter

            KeyboardButton {
                letter: "Capslock"
                width: 70
                onPressed: {
                    console.log("caps")
                    caps = !caps
                    remove(letter.length)
                }
            }

            KeyboardButton {letter: "A"}
            KeyboardButton {letter: "S"}
            KeyboardButton {letter: "D"}
            KeyboardButton {letter: "F"}
            KeyboardButton {letter: "G"}
            KeyboardButton {letter: "H"}
            KeyboardButton {letter: "J"}
            KeyboardButton {letter: "K"}
            KeyboardButton {letter: "L"}

        }

        RowLayout {

            spacing: 2
            Layout.alignment: Qt.AlignCenter

            KeyboardButton {letter: "Z"}
            KeyboardButton {letter: "X"}
            KeyboardButton {letter: "C"}
            KeyboardButton {letter: "V"}
            KeyboardButton {letter: "B"}
            KeyboardButton {letter: "N"}
            KeyboardButton {letter: "M"}

        }

        RowLayout {

            spacing: 2
            Layout.alignment: Qt.AlignCenter
            KeyboardButton {letter: " "; width: 200}
        }
    }


}
