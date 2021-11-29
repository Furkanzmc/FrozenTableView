import QtQuick 2.15
import QtQuick.Controls 2.15

Control {
    id: root

    property int cellIndex: -1
    property string text

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)
    leftPadding: 12
    rightPadding: 12
    contentItem: Label {
        leftPadding: 25
        rightPadding: 25
        text: root.text
        color: "white"
    }
    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 30
        color: "gray"
        border {
            width: 1
            color: "black"
        }
    }
    // Since we are using 4 different tables, we need to properly inform the table the size of our
    // column so that the rest of the tables' cells can adjust their own size.
    onImplicitWidthChanged: {
        TableView.view._updateColumnWidth(cellIndex, implicitWidth)
    }
    onImplicitHeightChanged: {
        TableView.view._updateHorizontalHeaderHeight(implicitHeight)
    }
    onWidthChanged: {
        TableView.view._updateColumnWidth(cellIndex, width)
    }
    onHeightChanged: {
        TableView.view._updateHorizontalHeaderHeight(height)
    }
    TableView.onPooled: {
        root.enabled = false
    }
    TableView.onReused: {
        root.enabled = true
    }
    Component.onCompleted: {
        if (cellIndex === -1) {
            console.log(
                "WARNING [Templates.Alias.Controls.TableViewHeaderCell]: cellIndex is not set. Column sizing will not work."
            )
        }
    }
}

