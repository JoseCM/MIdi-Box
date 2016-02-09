import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4

Window {

    id: mainWindow
    objectName: "ui"
    visible: true
    color: "lightsteelblue"
    minimumHeight: 640
    minimumWidth: 480
    maximumHeight: 640
    maximumWidth: 480

    signal addBlockSignal(int chain, int block, int type)
    signal removeBlockSignal(int chain, int block)
    signal addChainSignal(int input, int inputchannel,  int output, int outputchanel)
    signal removeChainSignal(int position)
    signal octaveUp()
    signal octaveDown()
    signal play()
    signal stop()
    signal bpm(int bpm)
    signal armChain(int pos)
    signal disarmChain(int pos)

    signal menusDisappear()

    MouseArea {

        anchors.fill: parent
        propagateComposedEvents: true
        onClicked: {
            menusDisappear()
        }

    }


    Button {
        text: "Add Chain"
        anchors.bottom: chainBox.top
        anchors.bottomMargin: 20
        anchors.left: chainBox.left
        onClicked: {
//            inputType.currentIndex = 0
//            inputChannel.currentIndex = 0
//            outputType.currentIndex = 0
//            outputChannel.currentIndex = 0
            addChainDialog.visible = true
            dialogMouseArea.enabled = true
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

        MouseArea {

            anchors.fill: parent
            propagateComposedEvents: true
            onClicked: {
                menusDisappear()
            }

        }

        ListView {
            id: chainView
            //clip: true
            //anchors.fill: parent
            width: parent.width
            height: contentHeight
            delegate: Chain { chainID: chainid }
            model: ListModel {
                id: chainModel
            }

        }

    }


    Label {
        text: "BPM: "
        anchors.right: bpmSpin.left
        anchors.top: bpmSpin.top
        anchors.rightMargin: 5
        anchors.topMargin: 2
    }

    SpinBox {
        id: bpmSpin
//        anchors.bottom: parent.bottom
//        anchors.left: parent.left
//        anchors.leftMargin: 225
//        anchors.margins: 25
        x: 175
        y: 588
        value: 120
        maximumValue: 250
        minimumValue: 50
        scale: 1.25

//        onEditingFinished: {
//            mainWindow.bpm(value)
//            console.log("nem bpm! ", value)
//        }

        onValueChanged: {
            mainWindow.bpm(value)
            console.log("nem bpm! ", value)
        }
    }



    MouseArea {
        id: playMouseArea
        anchors.fill: parent
        enabled: false
    }

    Rectangle {
        id: playButton
        x: 50
        y: 575
        width: 50
        height: 50
        color: "transparent"
        property int playing: 0

        Image {
            id: playImage
            source: "images/play.png"
            anchors.fill: parent
            z: - 1
        }


        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("play clicked")
                if(playButton.playing == 0){
                    playImage.source = "images/stop.png"
                    playButton.playing = 1
                    playMouseArea.enabled = true
                    mainWindow.play()
                    console.log("playing")
                } else {
                    playImage.source = "images/play.png"
                    playButton.playing = 0
                    playMouseArea.enabled = false
                    mainWindow.stop()
                    console.log("stoping")
                }
            }
        }

    }

    MouseArea {
        id: dialogMouseArea
        anchors.fill: parent
        enabled: false
    }

    Rectangle {

         id: addChainDialog
         objectName: "addChain"
         width: 320
         height: 250
         visible: false
         anchors.centerIn: parent
         z: 100

         border.color: "black"
         border.width: 2

         property int chaincount: 0

         function getChainPos(id){

             for(var i = 0; i < chainModel.count; i++)
                if(chainModel.get(i).chainid == id)
                    return i

             return -1

         }

         function addChain() {

             var input = tabPositionGroup.current.index
             var output = tabPositionGroup2.current.index
             var inchannel =  inputChannel.value
             var outchannel = outputChannel.value

             //var array = outputChannel.model
             //arrayay.splice(3, 1)
             //outputChannel.model = array

             chainModel.append({"chainid": addChainDialog.chaincount})
             chainView.currentIndex = chainModel.count - 1
             mainWindow.addChainSignal(input, inchannel , output, outchannel)
             console.log("adding chain..")

             chainView.currentItem.addBlock(0,"IN\n" + tabPositionGroup.current.text + "-" + inputChannel.value.toString())
             chainView.currentItem.addBlock(1, "OUT\n" + tabPositionGroup2.current.text + "-" + outputChannel.value.toString())

             addChainDialog.chaincount = addChainDialog.chaincount + 1

         }

         function removeChain(id) {

             var pos = addChainDialog.getChainPos(id)

             if(pos != -1){
                 chainModel.remove(pos, 1)
                 mainWindow.removeChainSignal(pos)
                 console.log("removing chain ", pos)
             }
         }


         GroupBox {
             id: inputType
             title: "INPUT:"
             x: 50
             y: 20

             ColumnLayout {
                 ExclusiveGroup { id: tabPositionGroup }
                 RadioButton {
                     property int index: 0
                     text: "MIDI"
                     checked: true
                     exclusiveGroup: tabPositionGroup
                 }
                 RadioButton {
                     property int index: 1
                     text: "USB"
                     exclusiveGroup: tabPositionGroup
                 }
                 RadioButton {
                     property int index: 2
                     text: "PHYS"
                     exclusiveGroup: tabPositionGroup
                 }
             }
         }

         Label {
             id: inputLabel
             anchors.top: inputType.bottom
             anchors.left: inputType.left
             anchors.margins: 5
             text: "Channel: "
         }

         SpinBox {
             id: inputChannel
             anchors.top: inputLabel.bottom
             anchors.left: inputLabel.left
             anchors.topMargin: 5
             maximumValue: 16
             minimumValue: 1
             activeFocusOnPress: false
         }

         GroupBox {
             id: outputType
             title: "OUTPUT:"
             anchors.left: inputType.right
             anchors.leftMargin: 50
             anchors.top: inputType.top
             ColumnLayout {
                 ExclusiveGroup { id: tabPositionGroup2 }
                 RadioButton {
                     property int index: 0
                     text: "MIDI"
                     checked: true
                     exclusiveGroup: tabPositionGroup2
                 }
                 RadioButton {
                     property int index: 1
                     text: "USB"
                     exclusiveGroup: tabPositionGroup2
                 }
             }
         }

         Label {
             id: outputLabel
             anchors.top: inputLabel.top
             anchors.left: outputType.left
             anchors.topMargin: 0
             anchors.margins: 5
             text: "Channel: "
         }

         SpinBox {
             id: outputChannel
             anchors.top: outputLabel.bottom
             anchors.left: outputLabel.left
             anchors.topMargin: 5
             maximumValue: 16
             minimumValue: 1
             activeFocusOnPress: false
         }

         Button {
             text: "OK"
             anchors.right: cancelButton.left
             anchors.rightMargin: 10
             anchors.top: cancelButton.top
             onClicked: {
                 addChainDialog.addChain()
                 addChainDialog.visible = false
                 dialogMouseArea.enabled = false
             }
         }

         Button {
             id: cancelButton
             text: "Cancel"
             x: 200
             y: 200
             onClicked: {
                 addChainDialog.visible = false
                 dialogMouseArea.enabled = false
            }
         }


     }

    GroupBox {

        x: 255
        y: 568
        scale: 0.8

        title: "OCTAVE"

        Button {
            id: octup
            text: "UP"
            onClicked: {
                console.log("octave up!")
                mainWindow.octaveUp()
            }
        }

        Button {
            id: octdown
            anchors.left: octup.right
            anchors.top: octup.top
            anchors.leftMargin: 10
            text: "DOWN"
            onClicked: {
                console.log("octave down!")
                mainWindow.octaveDown()
            }
        }
    }


//    Rectangle {

//        id: recordBox
//        color: "transparent"
//        border.color: "black"
//        border.width: 2
//        radius: 5

//        height: 500
//        width: 420
//        anchors.centerIn: parent

//    }

}
