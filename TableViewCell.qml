import QtQuick 2.15
import QtQuick.Controls 2.15

Control {
    id: root

    property string text

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)
    leftPadding: 12
    rightPadding: 12
    contentItem: Label {
        text: root.text
        color: "black"
    }
    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 30
        color: "transparent"
        border {
            width: 1
            color: "black"
        }
    }
    TableView.onPooled: {
        root.enabled = false
    }
    TableView.onReused: {
        root.enabled = true
    }
}

