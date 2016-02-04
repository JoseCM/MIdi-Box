import QtQuick 2.3
import QtQuick.Controls 1.4


Rectangle {

    property int blockID
    property int chainID
    property string blocklabel

    id: block
    color: "orange"
    border.color: "black"
    border.width: 2
    radius: 5
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.margins: 5
    anchors.rightMargin: 5
    anchors.leftMargin: 5
    width: 80

    Menu {

        id: blockMenu

        MenuItem {
            text: "Add Block to Right"
//            onTriggered: ...
        }

        MenuItem {
            text: "Add Block to Left"
//            onTriggered: ...
        }

        MenuItem {
            text: "Remove Block"
        }

        MenuItem {
            text: "Edit Block"
        }

        MenuItem {
            text: "Remove Chain"
        }

    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            blockMenu.popup()
        }

        onPressAndHold: {
            //shows block dialog
        }
    }

    Text {
        anchors.centerIn: parent
        text: blocklabel
        horizontalAlignment: Text.AlignHCenter
    }
}
