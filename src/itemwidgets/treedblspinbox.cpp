#include "paramdoublespinbox.h"
#include "paramtreemodel.h"
#include "paramtreeitem.h"
#include "widgetinitializers.h"
#include <QAbstractItemModel>

ParamDoubleSpinBox::ParamDoubleSpinBox(ParamTreeModel* model,const TreeItem& item,QWidget* parent) :
    QDoubleSpinBox(parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    ParamWidgetInitializer::initializeSpinBox(this,m_index);

    connect(this,QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double val) {
        m_model->setData(m_index,val,Qt::EditRole);
    });
    connect(m_model,&QAbstractItemModel::dataChanged,this,&ParamDoubleSpinBox::setEditorData);
}

void ParamDoubleSpinBox::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft){
        double value = m_index.data(Qt::DisplayRole).toDouble();
        QDoubleSpinBox::setValue(value);
    }
}

QString ParamDoubleSpinBox::name() const
{
    return m_name;
}

