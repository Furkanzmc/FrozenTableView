#ifndef MODEL_SLICE_H
#define MODEL_SLICE_H

#include <QAbstractListModel>

class ModelSlice : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int fromRow READ fromRow WRITE setFromRow NOTIFY fromRowChanged)
    Q_PROPERTY(int toRow READ toRow WRITE setToRow NOTIFY toRowChanged)
    Q_PROPERTY(int fromColumn READ fromColumn WRITE setFromColumn NOTIFY
                 fromColumnChanged)
    Q_PROPERTY(
      int toColumn READ toColumn WRITE setToColumn NOTIFY toColumnChanged)
    Q_PROPERTY(QAbstractItemModel* source READ source WRITE setSource NOTIFY
                 sourceChanged)

public:
    explicit ModelSlice(QObject* parent = nullptr);

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override final;
    [[nodiscard]] int rowCount(
      const QModelIndex& parent = QModelIndex{}) const override;
    [[nodiscard]] int columnCount(
      const QModelIndex& parent = QModelIndex{}) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index,
                                int role) const override;
    [[nodiscard]] QVariant headerData(
      int section,
      Qt::Orientation orientation,
      int role = Qt::DisplayRole) const override final;

    void setFromRow(int value);
    [[nodiscard]] int fromRow() const;

    void setToRow(int value);
    [[nodiscard]] int toRow() const;

    void setFromColumn(int value);
    [[nodiscard]] int fromColumn() const;

    void setToColumn(int value);
    [[nodiscard]] int toColumn() const;

    void setSource(QAbstractItemModel* model);
    [[nodiscard]] QAbstractItemModel* source() const;

signals:
    void countChanged();
    void fromRowChanged();
    void toRowChanged();
    void fromColumnChanged();
    void toColumnChanged();
    void sourceChanged();

protected:
    QModelIndex getTargetIndex(const QModelIndex& index) const;
    bool isValidIndex(const QModelIndex& targetIndex) const;

private:
    QAbstractItemModel* m_source{ nullptr };
    int m_fromRow{ -1 };
    int m_toRow{ -1 };
    int m_fromColumn{ -1 };
    int m_toColumn{ -1 };

private:
    void initConnections();
};

class HeaderModelSlice : public ModelSlice {
    Q_OBJECT

    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation
                 NOTIFY orientationChanged)
    Q_PROPERTY(bool useExplicitRange READ useExplicitRange WRITE
                 setUseExplicitRange NOTIFY useExplicitRangeChanged)

public:
    explicit HeaderModelSlice(QObject* parent = nullptr);

    [[nodiscard]] QVariant data(const QModelIndex& index,
                                int role) const override final;
    [[nodiscard]] int rowCount(
      const QModelIndex& parent = QModelIndex{}) const override final;
    [[nodiscard]] int columnCount(
      const QModelIndex& parent = QModelIndex{}) const override final;

    [[nodiscard]] Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation value);

    [[nodiscard]] bool useExplicitRange() const;
    void setUseExplicitRange(bool value);

signals:
    void orientationChanged(QPrivateSignal);
    void useExplicitRangeChanged(QPrivateSignal);

private:
    bool m_useExplicitRage{ false };
    Qt::Orientation m_orientation{ Qt::Orientation::Horizontal };

private:
    void updateRange();
};

#endif
