import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

Window {

    visible: true
    color: "lightsteelblue"
    minimumHeight: 640
    minimumWidth: 480
    maximumHeight: 640
    maximumWidth: 480

    Button {
        text: "Add Chain"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 20
    }

    Rectangle {

        id: chainBox
        objectName: "chainBox"
        color: "yellow"

        border.color: "black"
        border.width: 2
        radius: 5

        height: 500
        width: 420
        anchors.centerIn: parent

        ColumnLayout {
            id: chainColumns
            objectName: "chainColumns"
            spacing: 1
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            width: parent.width

            /*
            Chain {
                objectName: "chain1"
                Block {

                }
            }


            Chain {
                objectName: "chain2"
                color: "blue"
                Layout.row: 2
            }
            */

        }


    }


}
