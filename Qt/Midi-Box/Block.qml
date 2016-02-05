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
            onTriggered: chain.addBlockRight(chainID, blockID, "")
        }

        MenuItem {
            text: "Add Block to Left"
            onTriggered: chain.addBlockLeft(chainID, blockID, "")
        }

        MenuItem {
            text: "Remove Block"
            onTriggered: chain.removeBlock(chainID, blockID)
        }

        MenuItem {
            text: "Edit Block"
//            onTriggered:
        }

        MenuItem {
            text: "Remove Chain"
            onTriggered: addChainDialog.removeChain(chainID)
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
