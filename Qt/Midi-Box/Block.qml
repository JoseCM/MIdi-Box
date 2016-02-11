import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1


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


//    ListMenu {
//        id: blockMenu
//        height: 5 * 30
//        z: 2000

//        ColumnLayout {

//            anchors.fill: parent
//            spacing: 0

//            ListMenuItem {
//                txt: "Add Block to Right"
//                onSelected: {
//                    chain.addBlockRight(chainID, blockID, "")
//                    blockMenu.visible = false
//                }
//            }

//            ListMenuItem {
//                txt: "Add Block to Left"
//                onSelected: {
//                    chain.addBlockLeft(chainID, blockID, "")
//                    blockMenu.visible = false
//                }
//            }

//            ListMenuItem {
//                txt: "Remove Block"
//                onSelected: {
//                    chain.removeBlock(chainID, blockID)
//                    blockMenu.visible = false
//                }
//            }

//            ListMenuItem {
//                txt: "Edit Block"
//                onSelected: {
//                    // edit block
//                    blockMenu.visible = false
//                }
//            }

//            ListMenuItem {
//                txt: "Remove Chain"
//                onSelected: {
//                    addChainDialog.removeChain(chainID)
//                    blockMenu.visible = false
//                }
//            }

//        }

//    }


    MouseArea {
        anchors.fill: parent
        onClicked: {
            mainWindow.menusDisappear()
            mainWindow.setBlock(chainID, blockID)
            blockMenu.x = mouseX + parent.x + parent.parent.x + parent.parent.parent.x + 30
            blockMenu.y = mouseY + parent.y + parent.parent.y + parent.parent.parent.y + 70
            blockMenu.visible = true
        }
    }

    Text {
        anchors.centerIn: parent
        text: blocklabel
        horizontalAlignment: Text.AlignHCenter
    }
}
