#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>

namespace paramtree{

class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ItemDelegate(QWidget* parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    void setEditorData(QWidget* editor,const QModelIndex& index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget* editor,const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;

    mutable QComboBox* m_combo_box;

private slots:
    void setComboData(int val);
};

}

#endif // PARAMITEMDELEGATE_H
