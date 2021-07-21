#include "itemwidgets/treecheckbox.h"
#include "modelview/treemodel.h"
#include "modelview/treeitem.h"

namespace paramtree{

TreeCheckBox::TreeCheckBox(TreeModel* model,const TreeItem& item,QWidget* parent) :
    QCheckBox("",parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    QCheckBox::setTristate(false);
    QCheckBox::setCheckState(item.value().toBool() ? Qt::Checked : Qt::Unchecked);

    connect(this,QOverload<const int>::of(&QCheckBox::stateChanged),this,&TreeCheckBox::updateModel);
    connect(m_model,&QAbstractItemModel::dataChanged,this,&TreeCheckBox::setEditorData);
}

QString TreeCheckBox::name() const {
    return m_name;
}

void TreeCheckBox::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft){
        Qt::CheckState state = static_cast<Qt::CheckState>(m_index.data(Qt::CheckStateRole).toUInt());
        QCheckBox::setCheckState(state);
    }
}

void TreeCheckBox::updateModel(int val) {
    m_model->setData(m_index,static_cast<Qt::CheckState>(val),Qt::CheckStateRole);
}

}

