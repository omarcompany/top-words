import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Dialogs 1.0
import QtQuick.Controls 2.12
import Top.Words 1.0

ApplicationWindow {
    id: mainWindow

    minimumWidth: 640
    minimumHeight: 760

    visible: true
    title: qsTr("Top words")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            Action {
                text: qsTr("Open")
                onTriggered: {
                    loader.active = false
                    fileDialog.visible = true
                }
            }
        }
    }

    FileDialog {
        id: fileDialog

        title: "Please choose a file"
        folder: shortcuts.home
        nameFilters: ["Text files (*.txt)"]
        onAccepted: {
            loader.active = true
            WordsModel.openFile(fileUrl.toString().replace("file://",""))
        }
    }

    Loader {
        id: loader

        width: mainWindow.width
        height: mainWindow.height
        sourceComponent: histogramViewComponent
    }

    Component {
        id: histogramViewComponent

        HistogramView {
            width: parent.width
            height: parent.height
        }
    }
}
