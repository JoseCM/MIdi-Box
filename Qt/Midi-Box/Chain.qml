import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Rectangle {

        id: chain
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

        function addBlock(pos, text) {
            blockModel.insert(pos, {"blockid": blockcount, "chainid": chain.chainID, "textstr": text})
            blockcount = blockcount + 1
        }

        function removeBlock(pot){

        }

        function removeChain(id) {
            for(var i = 0; i < chainList.count; i++){
               if(chainList.get(i).chainid == chainID){
                   console.log("remove chain")
                   chainList.remove(i)
                   break
               }
            }
        }

        ListView {
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
                        addBlock(chainMenu.count - 1, "BLdsf")
                    }
                }

                MenuItem {
                    text: "Remove Chain"
                    onTriggered: {
                        chain.removeChain(chainID)
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
