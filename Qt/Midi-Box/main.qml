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
        color: "yellow"

        border.color: "black"
        border.width: 2
        radius: 5

        height: 500
        width: 420
        anchors.centerIn: parent

        GridLayout {

            id: colunas
            rowSpacing: 0
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            Chain {
                Block {

                }
            }

            Chain {
                color: "blue"
                Layout.row: 2
            }

        }

        Chain {
            parent: colunas
            color: "green"
            Layout.row: 1
        }

    }
}
