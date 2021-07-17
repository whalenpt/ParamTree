#ifndef TREEDBLSPINBOX_H
#define TREEDBLSPINBOX_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QModelIndex>

namespace paramtree {
class TreeModel;
class TreeItem;

class TreeDblSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    TreeDblSpinBox(TreeModel* model,const TreeItem& item,QWidget* parent=nullptr);
    QString name() const;

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    TreeModel* m_model;
    QModelIndex m_index;
};

}

#endif // PARAMDOUBLESPINBOX_H
