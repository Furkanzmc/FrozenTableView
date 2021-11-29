#include "ModelSlice.h"

#include <algorithm>

ModelSlice::ModelSlice(QObject* parent)
  : QAbstractListModel{ parent }
{
}

QHash<int, QByteArray> ModelSlice::roleNames() const
{
    return m_source->roleNames();
}

int ModelSlice::rowCount(const QModelIndex& parent) const
{
    if (m_fromRow == -1 || m_toRow == -1 || !m_source) {
        return !m_source ? 0 : m_source->rowCount(parent);
    }

    const int sourceCount{ m_source->rowCount(parent) };
    return std::min(std::min(m_toRow, sourceCount - 1) - m_fromRow + 1,
                    sourceCount);
}

int ModelSlice::columnCount(const QModelIndex& parent) const
{
    if (m_fromColumn == -1 || m_toColumn == -1 || !m_source) {
        return !m_source ? 0 : m_source->columnCount(parent);
    }

    const int sourceCount{ m_source->columnCount(parent) };
    return std::min(std::min(m_toColumn, sourceCount - 1) - m_fromColumn + 1,
                    sourceCount);
}

QModelIndex ModelSlice::getTargetIndex(const QModelIndex& index) const
{
    assert(m_source);
    return m_source->index(std::max(m_fromRow, 0) + index.row(),
                           std::max(m_fromColumn, 0) + index.column());
}

bool ModelSlice::isValidIndex(const QModelIndex& targetIndex) const
{
    if ((m_fromRow != -1 && targetIndex.row() < m_fromRow) &&
        (m_toRow != -1 && targetIndex.row() > m_toRow)) {
        return false;
    }

    if ((m_fromColumn != -1 && targetIndex.column() < m_fromColumn) &&
        (m_toColumn != -1 && targetIndex.column() > m_toColumn)) {
        return false;
    }

    return true;
}

QVariant ModelSlice::data(const QModelIndex& index, int role) const
{
    assert(m_source);
    const QModelIndex targetIndex{ getTargetIndex(index) };
    if (!isValidIndex(targetIndex)) {
        return {};
    }

    return m_source->data(targetIndex, role);
}

QVariant ModelSlice::headerData(int section,
                                Qt::Orientation orientation,
                                int role) const
{
    assert(m_source);
    return m_source->headerData(section, orientation, role);
}

void ModelSlice::setFromRow(int value)
{
    // TODO: Test updating interactively.
    if (m_fromRow == value) {
        return;
    }

    m_fromRow = value;
    emit fromRowChanged();
}

int ModelSlice::fromRow() const
{
    return m_fromRow;
}

void ModelSlice::setToRow(int value)
{
    // TODO: Test updating interactively.
    if (m_toRow == value) {
        return;
    }

    m_toRow = value;
    emit toRowChanged();
}

int ModelSlice::toRow() const
{
    return m_toRow;
}

void ModelSlice::setFromColumn(int value)
{
    // TODO: Test updating interactively.
    if (m_fromColumn == value) {
        return;
    }

    m_fromColumn = value;
    emit fromColumnChanged();
}

int ModelSlice::fromColumn() const
{
    return m_fromColumn;
}

void ModelSlice::setToColumn(int value)
{
    // TODO: Test updating interactively.
    if (m_toColumn == value) {
        return;
    }

    m_toColumn = value;
    emit toColumnChanged();
}

int ModelSlice::toColumn() const
{
    return m_toColumn;
}

void ModelSlice::setSource(QAbstractItemModel* model)
{
    if (m_source == model) {
        return;
    }

    beginResetModel();

    if (m_source) {
        disconnect(m_source, nullptr, this, nullptr);
    }

    m_source = model;
    if (m_source) {
        initConnections();
    }

    emit sourceChanged();

    endResetModel();
}

QAbstractItemModel* ModelSlice::source() const
{
    return m_source;
}

void ModelSlice::initConnections()
{
    assert(m_source);

    connect(m_source,
            &QAbstractItemModel::modelAboutToBeReset,
            this,
            &ModelSlice::modelAboutToBeReset,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::modelReset,
            this,
            &ModelSlice::modelReset,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::rowsAboutToBeMoved,
            this,
            &ModelSlice::rowsAboutToBeMoved,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::rowsMoved,
            this,
            &ModelSlice::rowsMoved,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::rowsAboutToBeInserted,
            this,
            &ModelSlice::rowsAboutToBeInserted,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::rowsInserted,
            this,
            &ModelSlice::rowsInserted,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &ModelSlice::rowsAboutToBeRemoved,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::rowsRemoved,
            this,
            &ModelSlice::rowsRemoved,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::columnsAboutToBeMoved,
            this,
            &ModelSlice::columnsAboutToBeMoved,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::columnsMoved,
            this,
            &ModelSlice::columnsMoved,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::columnsAboutToBeInserted,
            this,
            &ModelSlice::columnsAboutToBeInserted,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::columnsInserted,
            this,
            &ModelSlice::columnsInserted,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::columnsAboutToBeRemoved,
            this,
            &ModelSlice::columnsAboutToBeRemoved,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::columnsRemoved,
            this,
            &ModelSlice::columnsRemoved,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::layoutAboutToBeChanged,
            this,
            &ModelSlice::layoutAboutToBeChanged,
            Qt::UniqueConnection);
    connect(m_source,
            &QAbstractItemModel::layoutChanged,
            this,
            &ModelSlice::layoutChanged,
            Qt::UniqueConnection);
}

HeaderModelSlice::HeaderModelSlice(QObject* parent)
  : ModelSlice{ parent }
{
    connect(this, &HeaderModelSlice::sourceChanged, this, [&self = *this]() {
        self.updateRange();
    });
}

QVariant HeaderModelSlice::data(const QModelIndex& index, int role) const
{
    assert(source());
    assert(hasIndex(index.row(), index.column()));
    const QModelIndex targetIndex{ getTargetIndex(index) };
    assert(targetIndex.row() > -1 && targetIndex.column() > -1);

    return source()->headerData(m_orientation == Qt::Orientation::Horizontal
                                  ? targetIndex.column()
                                  : targetIndex.row(),
                                m_orientation,
                                role);
}

int HeaderModelSlice::rowCount(const QModelIndex& parent) const
{
    const int count{ ModelSlice::rowCount(parent) };
    return m_orientation == Qt::Orientation::Horizontal && source() ? 1 : count;
}

int HeaderModelSlice::columnCount(const QModelIndex& parent) const
{
    const int count{ ModelSlice::columnCount(parent) };
    return m_orientation == Qt::Orientation::Vertical && source() ? 1 : count;
}

Qt::Orientation HeaderModelSlice::orientation() const
{
    return m_orientation;
}

void HeaderModelSlice::setOrientation(Qt::Orientation value)
{
    if (m_orientation == value) {
        return;
    }

    m_orientation = value;
    updateRange();
    emit orientationChanged(QPrivateSignal{});
}

void HeaderModelSlice::updateRange()
{
    if (!source() || m_useExplicitRage) {
        return;
    }

    if (m_orientation == Qt::Orientation::Horizontal) {
        setFromRow(0);
        setToRow(0);
        setFromColumn(0);
        setToColumn(source()->columnCount());
    }
    else {
        setFromRow(0);
        setToRow(source()->rowCount());
        setFromColumn(0);
        setToColumn(0);
    }
}

bool HeaderModelSlice::useExplicitRange() const
{
    return m_useExplicitRage;
}

void HeaderModelSlice::setUseExplicitRange(bool value)
{
    if (m_useExplicitRage == value) {
        return;
    }

    m_useExplicitRage = value;
    updateRange();
    emit useExplicitRangeChanged(QPrivateSignal{});
}
