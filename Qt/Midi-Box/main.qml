import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

Window {

    id: mainWindow
    visible: true
    color: "lightsteelblue"
    minimumHeight: 640
    minimumWidth: 480
    maximumHeight: 640
    maximumWidth: 480

    Button {
        text: "Add Chain"
        anchors.bottom: chainBox.top
        anchors.bottomMargin: 20
        anchors.left: chainBox.left
        onClicked: {
            inputType.currentIndex = 0
            inputChannel.currentIndex = 0
            outputType.currentIndex = 0
            outputChannel.currentIndex = 0
            addChainDialog.show()
        }
    }

    Window {

        id: addChainDialog
        objectName: "addChain"
        minimumHeight: 200
        maximumHeight: 200
        minimumWidth: 320
        maximumWidth: 320
        modality: Qt.ApplicationModal

        property int chaincount: 0
        signal addChainSignal(int input, int inputchannel,  int output, int outputchanel)
        signal removeChainSignal(int position)

        function getChainPos(id){

            for(var i = 0; i < chainModel.count; i++)
               if(chainModel.get(i).chainid == id)
                   return i

            return -1

        }

        function addChain() {

            var input = inputType.currentIndex
            var output = outputType.currentIndex
            var inchannel =  parseInt(inputChannel.currentText)
            var outchannel = parseInt(outputChannel.currentText)

            //var array = outputChannel.model
            //arrayay.splice(3, 1)
            //outputChannel.model = array

            chainModel.append({"chainid": addChainDialog.chaincount})
            chainView.currentIndex = chainModel.count - 1
            chainView.currentItem.addBlock(0,"IN\n" + inputType.currentText + "-" + inputChannel.currentText)
            chainView.currentItem.addBlock(1, "OUT\n" + outputType.currentText + "-" + outputChannel.currentText)

            addChainDialog.chaincount = addChainDialog.chaincount + 1
            addChainDialog.hide()

            addChainDialog.addChainSignal(input, inchannel , output, outchannel)

        }

        function removeChain(id) {

            var pos = addChainDialog.getChainPos(id)

            if(pos != -1){
                chainModel.remove(pos, 1)
                addChainDialog.removeChainSignal(pos)
            }
        }

        Label {
            id: inputLabel
            text: "INPUT:"
            x: 50
            y: 20
        }

        ComboBox {
            id: inputType
            anchors.top: inputLabel.bottom
            anchors.left: inputLabel.left
            anchors.topMargin: 10
            model: ["MIDI", "USB", "PHYS"]
        }

        ComboBox {
            id: inputChannel
            anchors.top: inputType.bottom
            anchors.left: inputType.left
            anchors.topMargin: 10
            model: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 ,13, 14, 15, 16]
        }

        Label {
            id: outputLabel
            text: "OUTPUT:"
            anchors.left: inputLabel.right
            anchors.leftMargin: 80
            anchors.top: inputLabel.top
        }

        ComboBox {
            id: outputType
            anchors.top: outputLabel.bottom
            anchors.left: outputLabel.left
            anchors.topMargin: 10
            model: ["MIDI", "USB"]
        }

        ComboBox {
            id: outputChannel
            anchors.top: outputType.bottom
            anchors.left: outputType.left
            anchors.topMargin: 10
            model: [1 ,2 ,3 ,4 ,5 ,6 ,7 ,8, 9, 10, 11, 12, 13, 14, 15 ,16]
        }

        Button {
            text: "OK"
            anchors.right: cancelButton.left
            anchors.rightMargin: 10
            anchors.top: cancelButton.top
            onClicked: {
                addChainDialog.addChain()
            }
        }

        Button {
            id: cancelButton
            text: "Cancel"
            x: 200
            y: 150
            onClicked: addChainDialog.hide()
        }

    }

    Rectangle {

        id: chainBox
        objectName: "chainBox"
        color: "yellow"

        border.color: "black"
        border.width: 2
        radius: 5

        height: 500
        width: 420
        anchors.centerIn: parent

            ListView {
                id: chainView
                clip: true
                anchors.fill: parent
                delegate: Chain { chainID: chainid }
                model: ListModel {
                    id: chainModel
                }

            }
    }

    Rectangle {

        id: recordBox
        color: "transparent"
        border.color: "black"
        border.width: 2
        radius: 5

        height: 500
        width: 420
        anchors.centerIn: parent

    }



}
