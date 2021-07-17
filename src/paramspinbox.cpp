#include "paramspinbox.h"
#include "paramtreemodel.h"
#include "paramtreeitem.h"
#include "widgetinitializers.h"
#include <QAbstractItemModel>
#include <QDebug>

ParamSpinBox::ParamSpinBox(ParamTreeModel* model,const TreeItem& item,QWidget* parent) :
    QSpinBox(parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    ParamWidgetInitializer::initializeSpinBox(this,m_index);
    connect(this,QOverload<int>::of(&QSpinBox::valueChanged), [=](int val) {
        m_model->setData(m_index,val,Qt::EditRole);
    });
    connect(m_model,&QAbstractItemModel::dataChanged,this,&ParamSpinBox::setEditorData);
}

void ParamSpinBox::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft){
        int value = m_index.data(Qt::DisplayRole).toInt();
        QSpinBox::setValue(value);
    }
}

QString ParamSpinBox::name() const
{
    return m_name;
}


