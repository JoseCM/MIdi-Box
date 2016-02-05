import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

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

        property int chainID
        property int blockcount: 0
        property ListView blockView: blockView

        signal addBlockSignal(int chain, int block, int type)
        signal removeBlockSignal(int chain, int block)

        function getBlockPos(chain, block){

            var blockpos = blockModel.count
            var chainpos = chainModel.count

            for(var i = 0; i < chainModel.count; i++)
                if(chainModel.get(i).chainid == chain){
                    chainpos = i
                    console.log("chainpos ", chainpos )
                    break
                }

            if(chainpos < 0 || chainpos > chainModel.count -1)
                return

            for(var i = 0; i < blockModel.count; i++)
                if(blockModel.get(i).blockid == block){
                    blockpos = i
                    console.log("blockpos ", blockpos )
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
            blockModel.insert(pos, {"blockid": blockcount, "chainid": chain.chainID, "textstr": text})
            blockcount = blockcount + 1

            var chainpos = addChainDialog.getChainPos(chainID)

            chain.addBlockSignal(chainpos, pos, 0)
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
            chain.removeBlockSignal(chainpos, pos)
        }

         ListView {
            id: blockView
            clip: true
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

            Menu {

                id: chainMenu

                MenuItem {
                    text: "Add Block"
                    onTriggered: {
                        addBlock(blockModel.count - 1, "PROCESS")
                    }
                }

                MenuItem {
                    text: "Remove Chain"
                    onTriggered: {
                       addChainDialog.removeChain(chainID)
                    }
                }

            }

            MouseArea {
                z: - 1
                anchors.fill: parent
                onClicked: {
                    chainMenu.popup()
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
            width: 50

            Image {
                id: recordImage
                anchors.fill: parent
                scale: 0.5
                source: "images/recordoff.png"
            }

            MouseArea {
                anchors.fill: parent
            }

        }

        Rectangle {

            anchors.fill: parent
            color: "transparent"
            border.color: "black"
            border.width: 2

        }


}
