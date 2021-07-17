#include "paramcheckbox.h"
#include "paramtreemodel.h"
#include "paramtreeitem.h"

ParamCheckBox::ParamCheckBox(ParamTreeModel* model,const TreeItem& item,QWidget* parent) :
    QCheckBox("",parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    QCheckBox::setTristate(false);
    QCheckBox::setCheckState(item.value().toBool() ? Qt::Checked : Qt::Unchecked);

    connect(this,QOverload<const int>::of(&QCheckBox::stateChanged),this,&ParamCheckBox::updateModel);
    connect(m_model,&QAbstractItemModel::dataChanged,this,&ParamCheckBox::setEditorData);
}

QString ParamCheckBox::name() const {
    return m_name;
}

void ParamCheckBox::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft)
        QCheckBox::setCheckState(m_index.data(Qt::DisplayRole).toBool() ? Qt::Checked : Qt::Unchecked);
}

void ParamCheckBox::updateModel(int val) {
    Qt::CheckState state(static_cast<Qt::CheckState>(val));
    QVariant var(state == Qt::Checked ? QVariant(true) : QVariant(false));
    m_model->setData(m_index,var,Qt::EditRole);
}

