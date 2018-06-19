#ifndef RWTABLEDELEGATE_H
#define RWTABLEDELEGATE_H

#include <QStyledItemDelegate>

class RwTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RwTableDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;
signals:

public slots:
};

#endif // RWTABLEDELEGATE_H
