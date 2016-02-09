import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
//import QtQuick.Controls.Styles 1.4

Rectangle {

        id: chain
        objectName: "addBlock"
        color: "red"
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 60
        Layout.preferredWidth: parent.width
        border.color: "black"
        border.width: 2
        anchors.margins: 2
        radius: 5
        width: parent.width
        height: 60
        z: -chainID

        property int chainID
        property int blockcount: 0
        property ListView blockView: blockView

        function getBlockPos(chain, block){

            var blockpos = blockModel.count
            var chainpos = chainModel.count

            for(var i = 0; i < chainModel.count; i++)
                if(chainModel.get(i).chainid == chain){
                    chainpos = i
                    break
                }

            if(chainpos < 0 || chainpos > chainModel.count -1)
                return

            for(var i = 0; i < blockModel.count; i++)
                if(blockModel.get(i).blockid == block){
                    blockpos = i
                    break
                }

            return blockpos

        }

        function addBlockLeft(chain, block, text) {

            var chainpos = addChainDialog.getChainPos(chain)
            var blockpos = getBlockPos(chain, block)

            if(blockpos <= 0 || blockpos > blockModel.count -1)
                return

            chainView.currentIndex = chainpos
            chainView.currentItem.addBlock(blockpos, "LEFT")

        }

        function addBlockRight(chain, block, text) {

            var chainpos = addChainDialog.getChainPos(chain)
            var blockpos = getBlockPos(chain, block)

            if(blockpos < 0 || blockpos >= blockModel.count -1)
                return

            chainView.currentIndex = chainpos
            chainView.currentItem.addBlock(blockpos + 1, "RIGHT")

        }

        function addBlock(pos, text) {

            var chainpos = addChainDialog.getChainPos(chainID)

            if(pos != 0 && pos != blockModel.count){
                 mainWindow.addBlockSignal(chainpos, pos, 0)
                 console.log("adding block ", pos)
            }
            blockModel.insert(pos, {"blockid": blockcount, "chainid": chain.chainID, "textstr": text})
            blockcount = blockcount + 1


        }

        function removeBlock(chain, block){

            var chainpos = addChainDialog.getChainPos(chain)
            var blockpos = getBlockPos(chain, block)

            if(blockpos <= 0 || blockpos >= blockModel.count -1)
                return

           chainView.currentIndex = chainpos
           chainView.currentItem.remove(blockpos)

        }

        function remove(pos){
            blockModel.remove(pos, 1)

            var chainpos = addChainDialog.getChainPos(chainID)
            mainWindow.removeBlockSignal(chainpos, pos)
            console.log("removing block ", pos, " -chain ", chainpos)
        }

        Rectangle {

            anchors.fill: parent
            color: "transparent"
            border.color: "black"
            border.width: 2

        }


         ListView {
            id: blockView
            //clip: true
            anchors.fill: parent
            anchors.rightMargin: recordButton.width
            orientation: Qt.Horizontal
            spacing: 5
            leftMargin: 5
            rightMargin: 5

            delegate: Block { blockID: blockid; chainID: chainid; blocklabel: textstr}

            model: ListModel {
                id: blockModel
            }


            MouseArea {
                z: - 1
                anchors.fill: parent
                anchors.leftMargin: recordButton.width
                onClicked:{
                    //chainMenu.popup()
                    mainWindow.menusDisappear()
                    chchainmenu.x = mouseX
                    chchainmenu.y = mouseY
                    chchainmenu.visible = true
                }

                onPressAndHold: {
                    //shows block dialog
                }

            }

        }

        Rectangle {

            id: recordButton
            color: "steelblue"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            border.color: "black"
            border.width: 2
            width: 80

            Image {
                id: recordImage
                anchors.fill: parent
                scale: 0.5
                source: "images/recordoff.png"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {

                    var pos = addChainDialog.getChainPos(chainID)

                    if(Qt.colorEqual(parent.color, "steelblue")){
                        parent.color = "red"
                        mainWindow.armChain(pos)
                        console.log("arm chain - ", pos)
                    } else {
                        parent.color = "steelblue"
                        mainWindow.disarmChain(pos)
                        console.log("disarm chain - ", pos)
                    }

                }
            }

        }



        ListMenu {
            id: chchainmenu
            height: 2 * 30
            z: 2000

            ColumnLayout {

                anchors.fill: parent
                spacing: 0

                ListMenuItem {
                    txt: "Add Block"
                    onSelected: {
                        addBlock(blockModel.count - 1, "PROCESS")
                        chchainmenu.visible = false
                    }
                }

                ListMenuItem {
                    txt: "Remove Chain"
                    onSelected: {
                        addChainDialog.removeChain(chainID)
                        chchainmenu.visible = false
                    }
                }
            }

        }



}
