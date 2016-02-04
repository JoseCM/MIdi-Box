import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

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

        height: 100
        width: 420
        anchors.centerIn: parent

//            ListView {

//                id: colunas
////                anchors.top: parent.top
////                anchors.left: parent.left
////                anchors.right: parent.right
////                width: parent.width
////                height: parent.height
//                anchors.fill: parent
//                delegate: Chain {}
//                focus: true
//                highlight: Rectangle { color: "purple"; radius: 5}
//                model: ListModel {

//                    ListElement {
//                        colorname: "black"
//                    }

//                }

            ListView {

                anchors.fill: parent
                delegate: Chain { color: clr }
                model: ListModel {
                    ListElement{ clr: "blue" }
                    ListElement{ clr: "red" }
//                    ListElement{}
                }

            }
    }


}
