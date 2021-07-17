#include "itemwidgets/treecombobox.h"
#include "modelview/treemodel.h"

namespace paramtree{

TreeComboBox::TreeComboBox(TreeModel* model,const TreeItem& item,QWidget* parent) :
    QComboBox(parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    QComboBox::addItems(m_model->data(m_index,TreeModel::Role::RANGE).toStringList());
    QComboBox::setCurrentText(m_model->data(m_index,Qt::DisplayRole).toString());

    connect(this,QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            [=] (const QString& text) { m_model->setData(m_index,text,Qt::EditRole);} );

    connect(m_model,&QAbstractItemModel::dataChanged,this,&TreeComboBox::setEditorData);
}

void TreeComboBox::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft){
        QString text = m_index.data(Qt::DisplayRole).toString();
        QComboBox::setCurrentText(text);
    }
}

QString TreeComboBox::name() const
{
    return m_name;
}


}

