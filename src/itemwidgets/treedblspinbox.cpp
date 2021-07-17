
#include "itemwidgets/treedblspinbox.h"
#include "shared/widgetinitializer.h"
#include "modelview/treemodel.h"
#include "modelview/treeitem.h"
#include <QAbstractItemModel>

namespace paramtree{

TreeDblSpinBox::TreeDblSpinBox(TreeModel* model,const TreeItem& item,QWidget* parent) :
    QDoubleSpinBox(parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    widgetinitializer::initializeSpinBox(this,m_index);
    connect(this,QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double val) {
        m_model->setData(m_index,val,Qt::EditRole);
    });
    connect(m_model,&QAbstractItemModel::dataChanged,this,&TreeDblSpinBox::setEditorData);
}

void TreeDblSpinBox::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft){
        double value = m_index.data(Qt::DisplayRole).toDouble();
        QDoubleSpinBox::setValue(value);
    }
}

QString TreeDblSpinBox::name() const
{
    return m_name;
}

}

