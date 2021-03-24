#ifndef PARAMITEMDELEGATE_H
#define PARAMITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>

class ParamItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ParamItemDelegate(QWidget* parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    void setEditorData(QWidget* editor,const QModelIndex& index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget* editor,const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;

    mutable QComboBox* combo_box;
    mutable QComboBox* m_bool_box;

private slots:
    void setComboData(int val);
    void setBoolData(int val);
};

#endif // PARAMITEMDELEGATE_H
