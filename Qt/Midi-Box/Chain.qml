import QtQuick 2.3
import QtQuick.Layouts 1.1

Rectangle {


        color: "red"
        border.color: "black"
        border.width: 2
        anchors.margins: 2
        radius: 5
        width: parent.width
        height: 60

        ListView {
            anchors.fill: parent
            orientation: Qt.Horizontal
            spacing: 10
            delegate: Block { color: clr }
            model: ListModel {
                id: blockModel
                ListElement {clr : "pink"}
            }
        }

        Rectangle {

            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 2
            width: 50

            Image {

                anchors.fill: parent
                source: "media-record.png"

            }
        }


}
