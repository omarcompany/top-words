import QtQuick 2.15
import QtQuick.Controls 2.12
import Top.Words 1.0

Item {
    id: root

    ProgressBar {
        id: progressBar

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: Style.margin
        }
        value: WordsModel.progress
        visible: WordsModel.status === WordsModel.Loading
    }

    HistogramItem {
        id: histogramItem

        anchors.top: progressBar.bottom

        width: parent.width
        height: parent.height - progressBar.height - gridView.height - Style.margin
    }

    GridView {
        id: gridView

        anchors {
            top: histogramItem.bottom
            topMargin: Style.margin * 2
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width - Style.margin * 4
        height: cellHeight * 10

        cellWidth: Style.margin * 6
        cellHeight: Style.margin

        model : WordsModel

        delegate: Row {
            spacing: 5
            Rectangle {
                id: rectangle

                width: height
                height: Style.margin / 2

                color: Style.colors[model.index]
                radius: width / 2
            }
            Label {
                anchors.verticalCenter: rectangle.verticalCenter
                width: gridView.cellWidth - Style.margin
                text: qsTr("%1").arg(model.text)
                elide: Text.ElideLeft
            }
        }
    }
}
