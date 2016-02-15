import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

Rectangle {

     id: monitorView
     objectName: "activitiesTable"
     anchors.centerIn: mainWindow
     height: 200
     width: 200

    TableView {
            id: activitiesTable
            anchors.fill: parent

            selectionMode: SelectionMode.NoSelection
            verticalScrollBarPolicy: 2
            sortIndicatorVisible: false
            horizontalScrollBarPolicy: 0


            TableViewColumn {
                id: channelCollumn
                width: (monitorView.width/4)
                objectName: "timeColumn"
                resizable: false
                movable: false
                role: "Time"
                title: "Channel"
            }

            TableViewColumn {
                id: messageColumn
                width: monitorView.width/4
                objectName: "activityColumn"
                resizable: false
                movable: false
                role: "Activity"
                title: "Message"
            }

            TableViewColumn {
                id: data1Column
                width: monitorView.width/4
                objectName: "activityColumn"
                resizable: false
                movable: false
                role: "Activity"
                title: "Data 1"
            }

            TableViewColumn {
                id: data2Column
                width: monitorView.width/4
                objectName: "activityColumn"
                resizable: false
                movable: false
                role: "Activity"
                title: "Data 2"
            }
    }

}
