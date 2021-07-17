#include "modelview/treemodel.h"
#include "modelview/treeitem.h"
#include "itemwidgets/treespinbox.h"
#include "shared/widgetinitializer.h"
#include <QAbstractItemModel>
#include <QDebug>

namespace paramtree{

TreeSpinBox::TreeSpinBox(TreeModel* model,const TreeItem& item,QWidget* parent) :
    QSpinBox(parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    widgetinitializer::initializeSpinBox(this,m_index);
    connect(this,QOverload<int>::of(&QSpinBox::valueChanged), [=](int val) {
        m_model->setData(m_index,val,Qt::EditRole);
    });
    connect(m_model,&QAbstractItemModel::dataChanged,this,&TreeSpinBox::setEditorData);
}

void TreeSpinBox::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft){
        int value = m_index.data(Qt::DisplayRole).toInt();
        QSpinBox::setValue(value);
    }
}

QString TreeSpinBox::name() const
{
    return m_name;
}

}


