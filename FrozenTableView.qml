import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Table 1.0

Control {
    id: root

    property TableModel model
    property alias delegate: tb.delegate
    // We are going to use the same delegate for our frozen and unfrozen column headers.
    property alias horizontalHeaderDelegate: columnHeader.delegate
    property alias columnSpacing: columnHeader.columnSpacing
    property alias rowSpacing: columnHeader.rowSpacing

    // NOTE: This is used to ensure that the frozen column table is wide enough to contain all the
    // frozen columns so we can properly calculate cell widths.
    property int defaultCellWidth: 100
    property int frozenColumn: -1

    // NOTE: ColumnLayout does not set implicit size so by default this will evaluate to 0 if
    // there's no padding.
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)
    contentItem: ColumnLayout {
        spacing: 0

        RowLayout {
            height: columnHeader.height
            spacing: 0
            Layout.fillWidth: true

            TableView {
                id: frozenColumnTable
                width: privates.frozenCellsCreated ? privates.frozenColumnWidth : root.defaultCellWidth * Math.max(root.frozenColumn - 1, 0)
                boundsBehavior: Flickable.StopAtBounds
                columnSpacing: columnHeader.columnSpacing
                rowSpacing: columnHeader.rowSpacing
                interactive: false
                columnWidthProvider: (column) => privates.columnWidths[column]
                model: HeaderModelSlice {
                    id: frozenColumnModel
                    source: root.model
                    useExplicitRange: true
                    fromRow: 0
                    toRow: 0
                    fromColumn: 0
                    toColumn: Math.max(root.frozenColumn - 1, 0)
                    orientation: Qt.Horizontal
                }
                delegate: root.horizontalHeaderDelegate
                Layout.preferredWidth: width
                Layout.preferredHeight: height

                // We need to duplicate these functions because a header cell that belongs to a
                // frozen column will have a different table view than the one that's not frozen.
                function _updateColumnWidth(index: int, width: int) {
                    privates.columnWidths[index] = width

                    if (!privates.frozenCellsCreated) {
                        privates.frozenCellsCreated = index == frozenColumnModel.toColumn
                    }

                    privates.frozenColumnWidth = privates.calculateFrozenColumnTableWidth()
                    Qt.callLater(frozenRows.forceLayout)
                    Qt.callLater(frozenColumnTable.forceLayout)
                }

                function _updateHorizontalHeaderHeight(height: int) {
                    frozenColumnTable.height = height
                    columnHeader.height = height
                }
            }

            TableView {
                id: columnHeader
                boundsBehavior: Flickable.StopAtBounds
                interactive: false
                clip: true
                columnWidthProvider: (column) => privates.columnWidths[root.frozenColumn > 0 ? column + root.frozenColumn : column]
                model: HeaderModelSlice {
                    source: root.model
                    useExplicitRange: true
                    fromRow: 0
                    toRow: 0
                    fromColumn: Math.max(root.frozenColumn, 0)
                    toColumn: root.model.columnCount
                    orientation: Qt.Horizontal
                }
                Layout.preferredHeight: height
                Layout.fillWidth: true

                function _updateColumnWidth(index: int, width: int) {
                    const columnIndex = root.frozenColumn > 0 ? index + root.frozenColumn : index
                    privates.columnWidths[columnIndex] = width

                    Qt.callLater(tb.forceLayout)
                    Qt.callLater(columnHeader.forceLayout)
                }

                function _updateHorizontalHeaderHeight(height: int) {
                    columnHeader.height = height
                }
            }
        }

        RowLayout {
            spacing: 0
            Layout.fillHeight: true
            Layout.fillWidth: true

            TableView {
                id: frozenRows
                width: frozenColumnTable.width
                boundsBehavior: Flickable.StopAtBounds
                columnSpacing: columnHeader.columnSpacing
                rowSpacing: columnHeader.rowSpacing
                contentY: tb.contentY
                clip: true
                syncView: tb
                syncDirection: Qt.Vertical
                delegate: tb.delegate
                columnWidthProvider: (column) => privates.columnWidths[column]
                model: ModelSlice {
                    source: root.model
                    fromRow: 0
                    toRow: root.model.rowCount
                    fromColumn: 0
                    toColumn: frozenColumnModel.toColumn
                }
                Layout.preferredWidth: width
                Layout.fillHeight: true
            }

            TableView {
                id: tb
                columnSpacing: columnHeader.columnSpacing
                rowSpacing: columnHeader.rowSpacing
                syncView: columnHeader.visible ? columnHeader : null
                syncDirection: Qt.Horizontal
                boundsBehavior: Flickable.StopAtBounds
                clip: true
                columnWidthProvider: (column) => privates.columnWidths[root.frozenColumn > 0 ? column + root.frozenColumn : column]
                model: ModelSlice {
                    source: root.model
                    fromRow: 0
                    toRow: root.model.rowCount
                    fromColumn: frozenColumnModel.toColumn
                    toColumn: root.model.columnCount
                }
                Layout.fillWidth: true
                Layout.fillHeight: true
                ScrollBar.vertical: ScrollBar { }
                ScrollBar.horizontal: ScrollBar { }
            }
        }
    }

    QtObject {
        id: privates

        property var columnWidths: ({})
        property int frozenColumnWidth: 0
        property bool frozenCellsCreated: false

        function calculateFrozenColumnTableWidth() {
            let column = frozenColumnModel.toColumn
            let width = 0
            while (column > -1) {
                const value = privates.columnWidths[column]
                if (value !== undefined) {
                    width += value
                }

                column--
            }

            return width
        }
    }
}

