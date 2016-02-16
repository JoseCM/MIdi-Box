import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

Rectangle {

     id: monitorView
     objectName: "monitorTable"
     anchors.centerIn: mainWindow
     height: 200
     width: 200

     visible: false

     function setModel(model){
         midiTable.model = model
     }

    TableView {

            id: midiTable
            anchors.fill: parent
            anchors.margins: 20
            anchors.bottomMargin: 50

            selectionMode: SelectionMode.NoSelection
            verticalScrollBarPolicy: 2
            sortIndicatorVisible: false
            horizontalScrollBarPolicy: 0


            TableViewColumn {
                id: channelCollumn
                width: monitorView.width/4
                resizable: false
                movable: false
                role: "Channel"
                title: "Channel"
            }

            TableViewColumn {
                id: messageColumn
                width: monitorView.width/4
                resizable: false
                movable: false
                role: "Message"
                title: "Message"
            }

            TableViewColumn {
                id: data1Column
                width: monitorView.width/4
                resizable: false
                movable: false
                role:  "Data 1"
                title: "Data 1"
            }

            TableViewColumn {
                id: data2Column
                width: monitorView.width/4
                resizable: false
                movable: false
                role: "Data 2"
                title: "Data 2"
            }
    }

    Button {

        text: "Exit"

        anchors.bottom: parent.bottom

        onClicked: {
            parent.visible = false
        }

    }

}
