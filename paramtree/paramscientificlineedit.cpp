#include "paramscientificlineedit.h"
#include "paramtreemodel.h"
#include "paramtreeitem.h"
#include "widgetinitializers.h"
#include <QAbstractItemModel>
#include <QDebug>

ParamScientificLineEdit::ParamScientificLineEdit(ParamTreeModel* model,const TreeItem& item,QWidget* parent) :
    ScientificLineEdit(parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    ScientificLineEdit::setValue(m_model->data(m_index,Qt::DisplayRole).toDouble());
    connect(this,&QLineEdit::editingFinished, [this] {
       m_model->setData(m_index,ScientificLineEdit::value(),Qt::EditRole);
    });
    connect(m_model,&QAbstractItemModel::dataChanged,this,&ParamScientificLineEdit::setEditorData);
}

void ParamScientificLineEdit::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft){
        double value = m_index.data(Qt::DisplayRole).toDouble();
        ScientificLineEdit::setValue(value);
    }
}

