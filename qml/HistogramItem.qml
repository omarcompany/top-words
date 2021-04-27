import QtQuick 2.15
import Top.Words 1.0

Item {
    id: root

    property int _scalabilityWidth: WordsModel.count < 7 ? 7 : WordsModel.count
    property real _contentWidth: width - Style.margin

    Row {
        id: row

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: Style.margin
        }

        spacing: Style.margin/2

        Repeater {
            model: WordsModel

            delegate: Rectangle {
                width: root._contentWidth/root._scalabilityWidth - parent.spacing
                height: root.height

                color: Style.lightGrey
                radius: width/3
                border.width: 1
                border.color: Style.lightGrey

                Rectangle {
                    id: coloredRectangle

                    width: parent.width
                    height: parent.height * model.count / WordsModel.maximumWords

                    color: Style.colors[model.index]
                    transform: Rotation { origin.x: width/2; origin.y: root.height/2; angle: 180}
                    radius: width/3

                    Behavior on height {
                        enabled: (WordsModel.status === WordsModel.Loading)

                        SpringAnimation { damping: 0.7; spring: 4}
                    }

                    PropertyAnimation {
                        target: coloredRectangle
                        property: "height"
                        duration: 400
                        from: 0
                        to: coloredRectangle.height
                        Component.onCompleted: { start() }
                    }

                }
                Text {
                    text: qsTr("%1").arg(model.count)
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: radius/2
                        horizontalCenter: parent.horizontalCenter
                    }
                }
                MouseArea {
                    id: mouseArea

                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: parent.border.color = Style.grey
                    onExited: parent.border.color = Style.lightGrey
                }
            }
        }
    }
}
