import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Table 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    FrozenTableView {
        anchors.fill: parent
        columnSpacing: 1
        rowSpacing: 1
        clip: true
        model: TableModel {
        }
        horizontalHeaderDelegate: TableViewHeaderCell {
            required property int index
            required property string display

            text: display
            cellIndex: index
        }
        delegate: TableViewCell {
            required property int index
            required property string display

            text: display
        }
        frozenColumn: 3
    }
}
