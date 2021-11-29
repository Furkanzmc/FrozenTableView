#include "TableModel.h"

#include <array>

namespace {
constexpr std::array<char, 26> s_characters{ 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                             'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                             'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                                             'V', 'W', 'X', 'Y', 'Z' };
}

TableModel::TableModel(QObject* parent)
  : QAbstractTableModel{ parent }
{
}

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    return { { Qt::DisplayRole, "display" } };
}

int TableModel::rowCount(const QModelIndex& /*index*/) const
{
    return m_rowCount;
}

int TableModel::columnCount(const QModelIndex& /*index*/) const
{
    return m_columnCount;
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            return QString{ "%1, %2" }.arg(index.column()).arg(index.row());
        default:
            break;
    }

    return {};
}

QVariant TableModel::headerData(int section,
                                Qt::Orientation orientation,
                                int role) const
{
    Q_UNUSED(orientation);
    switch (role) {
        case Qt::DisplayRole:
            return QString{ s_characters.at(section) };
        default:
            break;
    }

    return {};
}

QModelIndex TableModel::index(int row,
                              int column,
                              const QModelIndex& parent) const
{
    // NOTE: We are using the first item in our data as the header row.
    // That's why we need to add one to check for valid index.
    if (row < rowCount(parent) + 1 && column < columnCount(parent)) {
        return createIndex(row, column);
    }

    return QModelIndex{};
}
