#include "paramcombobox.h"
#include "paramtreemodel.h"
#include <QDebug>

ParamComboBox::ParamComboBox(ParamTreeModel* model,const TreeItem& item,QWidget* parent) :
    QComboBox(parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    QComboBox::addItems(m_model->data(m_index,ParamTreeModel::Role::RANGE).toStringList());
    QComboBox::setCurrentText(m_model->data(m_index,Qt::DisplayRole).toString());

    connect(this,QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            [=] (const QString& text) { m_model->setData(m_index,text,Qt::EditRole);} );

    connect(m_model,&QAbstractItemModel::dataChanged,this,&ParamComboBox::setEditorData);
}

void ParamComboBox::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft){
        QString text = m_index.data(Qt::DisplayRole).toString();
        QComboBox::setCurrentText(text);
    }
}

QString ParamComboBox::name() const
{
    return m_name;
}
