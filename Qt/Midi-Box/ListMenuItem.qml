import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {

    id: menuitem

    color: "steelblue"
    height: 30
    width: parent.width
    border.color: "black"
    border.width: 1
    z: 100
    focus: true

    property string txt

    signal selected

    Text {
        id: textitem
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        text: txt
    }

    MouseArea {
        anchors.fill: parent
        z: 200

//        onHoveredChanged: {
//            if(Qt.colorEqual(parent.color, "lightsteelblue"))
//                parent.color = "steelblue"
//            else
//                parent.color = "lightsteelblue"
//        }

        onClicked: {
            menuitem.selected()
            mainWindow.menusDisappear()
        }

    }

}
