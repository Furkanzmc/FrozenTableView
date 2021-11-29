#ifndef TABLE_MODEL_H
#define TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QByteArray>
#include <QHash>

class TableModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(int rowCount READ rowCount CONSTANT)
    Q_PROPERTY(int columnCount READ columnCount CONSTANT)

public:
    explicit TableModel(QObject* parent = nullptr);

    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] int rowCount(
      const QModelIndex& index = QModelIndex{}) const override;
    [[nodiscard]] int columnCount(
      const QModelIndex& index = QModelIndex{}) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index,
                                int role) const override;
    [[nodiscard]] QVariant headerData(
      int section,
      Qt::Orientation orientation,
      int role = Qt::DisplayRole) const override;

    QModelIndex index(
      int row,
      int column,
      const QModelIndex& parent = QModelIndex()) const override final;

private:
    int m_rowCount{ 200 };
    int m_columnCount{ 200 };
};

#endif
